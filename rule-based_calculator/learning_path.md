# Building a Rule-Based Calculator Service in C

> A teaching guide focused on *thinking* behind each layer — not just what to code, but **why**.

---

## What Is a "Rule-Based Calculator"?

Before anything else, understand the distinction:

- A **regular calculator** evaluates expressions: `2 + 3 = 5`
- A **rule-based calculator** applies *defined rules* to inputs:
  ```
  IF category == "food" THEN tax_rate = 0.06
  ```
  ...then computes from those rules.

Think of it like a mini policy engine that also does math. This is why it warrants a "service" — it's **stateful** and **long-running**.

---

## Phase 1: Design & Setup — Think Before You Code

### Define Your Rule Syntax First

This is the most important decision. Everything else depends on it. Ask yourself:

- What does a "rule" look like in text? Example:
  ```
  rule discount: IF quantity > 100 THEN price * 0.9
  ```
- What data types do you need? (integers, floats, booleans?)
- Do rules have priority/ordering? Can they chain?

> **Why this matters:** Your syntax decision drives your entire parser design. A loose syntax means a complex parser. A rigid syntax (like a fixed format) means a simple one.

---

### Establish the Project Skeleton

Set up folders *before* writing logic. The structure separates concerns:

```text
project-root/
├── include/
│   └── calculator.h    ← shared types, structs, function signatures
├── src/
│   ├── main.c          ← service entry point
│   ├── parser.c        ← rule definition and parsing
│   └── engine.c        ← logic execution
├── tests/
│   └── test_engine.c
├── Makefile
└── README.md
```

| Folder | Purpose |
|---|---|
| `include/` | Shared type definitions, structs, function signatures |
| `src/` | Actual implementations |
| `tests/` | Isolated test cases for each module |

Your `calculator.h` header is the **contract** between modules. Define your structs and function prototypes there **first** — before writing any `.c` files.

---

## Phase 2: Core Engine — The Hard Part

This has two sub-problems: **parsing** and **execution**.

---

### The Parser (`parser.c`)

Parsing turns a string like `"IF x > 5 THEN x * 2"` into a data structure your engine can work with.

#### Two Approaches to Learn

**1. Table-Driven Parser** — Good for simple, regular grammars
- You define a table of tokens and transitions
- Fast, low memory, but hard to extend
- Think of it like a state machine:
  ```
  START → READ_IF → READ_CONDITION → READ_THEN → READ_ACTION
  ```

**2. Recursive Descent Parser** — Good for nested/complex expressions
- You write one function per grammar rule
- `parse_expression()` calls `parse_term()` calls `parse_factor()`
- More code, but very readable and extensible

> **For beginners:** Start with a recursive descent parser. It maps cleanly to how you think about math grammar.

---

### The Execution Engine (`engine.c`)

Once parsed, your rule becomes a **tree of nodes** — called an **AST (Abstract Syntax Tree)**:

```
    IF
   /  \
  >    *
 / \  / \
x   5 x   2
```

Executing it means **walking the tree recursively**:

| Node Type | Action |
|---|---|
| Number literal | Return its value |
| `>`, `<`, `==` | Evaluate left and right sides, then compare |
| `+`, `*`, `-` | Evaluate both sides, apply the operator |
| `IF` | Evaluate condition → execute the matching branch |

This is called a **tree-walking interpreter** and it's the simplest execution model to build.

---

## Phase 3: Service Logic — Making It Long-Running

This is what elevates it from a "program" to a "service."

---

### The Main Loop (`main.c`)

Your `main()` shouldn't do calculation logic. It should:

1. Initialize the engine
2. Enter a loop: **read input → process → output result → repeat**
3. Handle shutdown cleanly

```
while (running) {
    read line from stdin
    pass to parser
    pass result to engine
    print output
}
```

---

### Signal Handling — Critical Concept

When a user hits `Ctrl+C`, the OS sends `SIGINT`. If you don't handle it, your service just dies — possibly mid-write, leaving corrupt state.

**You must register signal handlers:**

| Signal | Trigger | Action |
|---|---|---|
| `SIGINT` | User presses Ctrl+C | Set `running = 0` flag |
| `SIGTERM` | `kill` command sent | Set `running = 0` flag |

#### Why a Flag, Not `exit()` Directly?

Because you may need to:
- Flush output buffers
- Free allocated memory
- Write a final log entry
- Close file descriptors

> **Key insight:** Never do complex work inside a signal handler. Just set a flag and let your main loop react to it. A clean exit means the loop finishes its current iteration, sees `running == 0`, and shuts down gracefully.

---

## Phase 4: Testing & Hardening

### Test the Engine in Isolation

Your `test_engine.c` should test the engine **without** the service layer. Feed it pre-parsed rules directly. This is the principle of **unit testing** — test one thing at a time.

---

### Input Validation — Think About Failure First

Every string coming from `stdin` is hostile. Ask:

- What if the user types nothing?
- What if they type `IF IF IF`?
- What if a number overflows?

Your parser should return **error codes or NULL**, never crash. Your engine should always check those return values. This is called **defensive programming**.

---

## The Learning Path

Here's the order in which to learn and implement:

```
1. Structs & Enums in C         → represent rules as data
         ↓
2. String tokenization (strtok) → split input into tokens
         ↓
3. Recursive functions          → build the recursive descent parser
         ↓
4. Linked lists or arrays       → store multiple rules
         ↓
5. Function pointers            → dispatch execution by node type
         ↓
6. Signal handling (signal.h)   → clean shutdown
         ↓
7. Makefile                     → tie it all together
```

---

## The One Mental Model to Hold Onto

> **Every layer converts one representation into another.**

```
String input  →[parser]→  Tree structure  →[engine]→  Numeric result
```

If you keep that pipeline clear in your mind, you'll always know which file owns which responsibility — and your code will stay clean.