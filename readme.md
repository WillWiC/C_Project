# Building C Projects

---

## 0. C Tutorial
**Focus:** Basic tutorials to familiarise with the syntax and key concepts of C

---

## 1. Basic Calculator Service
**Focus:** Basics (Variables, I/O, Control Flow) → Library Design, Header Files, Memory Layout

A terminal-based arithmetic calculator that runs as a long-lived REPL service.
Expressions are entered interactively, parsed from infix to postfix notation via
the Shunting-Yard algorithm, then evaluated against a table-driven operator engine.

---

### Project Structure
```text
project-root/
├── include/
│   └── calculator.h    # All declarations — types, enums, structs, prototypes
├── src/
│   ├── main.c          # Entry point — REPL loop, signal handling, lifecycle
│   ├── parser.c        # Tokenisation, infix-to-postfix conversion
│   └── engine.c        # Arithmetic, operator table, evaluation, lifecycle
├── tests/
│   └── test_engine.c   # Unit tests using test hooks
├── Makefile
└── README.md
```

---

### Architecture

```
User input (string)
      │
      ▼
 calc_tokenize()        → Token[]          (lexer — parser.c)
      │
      ▼
 calc_infix_to_postfix() → Token[]         (Shunting-Yard — parser.c)
      │
      ▼
 calc_evaluate()        → calc_num_t       (stack evaluator — engine.c)
      │
      ▼
 printf result / calc_strerror()           (main.c)
```

Each stage is independent and testable in isolation.

---

### Public API

| Function | File | Purpose |
|---|---|---|
| `calc_init(ctx)` | engine.c | Initialise context and operator table |
| `calc_shutdown(ctx)` | engine.c | Zero out and clean up context |
| `calc_tokenize(ctx, input, tokens, count)` | parser.c | Lex raw input into a token array |
| `calc_infix_to_postfix(ctx, in, n, out, count)` | parser.c | Convert infix tokens to postfix order |
| `calc_evaluate(ctx, tokens, count, result)` | engine.c | Evaluate a postfix token array |
| `calc_strerror(err)` | engine.c | Convert an error code to a display string |
| `calc_ctx_reset_for_test(ctx)` | engine.c | Reset error state between expressions or tests |

---

### Key Concepts

**Data types**
`double` is aliased to `calc_num_t` via `typedef`. This centralises the numeric
type so the entire codebase can be switched to `float` or `long double` with one
line change in the header.

**Error handling**
Functions return a `calc_error_t` enum value — never a string, never a magic
number. The result of a computation is always written through an out-parameter.
This cleanly separates "did it succeed" from "what was the answer".

**Function pointers**
Each `Operator` in the table stores an `OperatorFunc` alongside its symbol,
precedence, and associativity. The evaluator calls `op->func(ctx, a, b, &result)`
without a `switch` statement — adding a new operator is one table row.

**Context pattern**
`calc_ctx_t` bundles all mutable engine state — the operator table, token buffer,
and error state. Every API function takes it as the first parameter. This keeps
functions reentrant and makes unit testing straightforward — each test gets its
own context.

**Forward declarations**
`OperatorFunc` takes a `calc_ctx_t*` parameter, but `calc_ctx_t` is defined
after it in the header. A forward declaration (`typedef struct calc_ctx_t calc_ctx_t`)
placed above `OperatorFunc` breaks the circular dependency without reordering the
entire header.

**Service loop**
`main.c` installs handlers for `SIGINT` and `SIGTERM` that set a `volatile
sig_atomic_t` flag. The REPL checks the flag each iteration and shuts down
cleanly rather than being killed mid-expression. `signal()` is used instead of
`sigaction()` for portability across POSIX and Windows.

---

### Implementation Order

1. **`calculator.h`** — define all types, enums, and prototypes first; nothing
   else compiles without it
2. **`engine.c`** — `calc_init`, `calc_shutdown`, arithmetic functions,
   `calc_get_operator`, `calc_evaluate`
3. **`parser.c`** — `whitespace_skipper`, `digit_collector`, `calc_tokenize`,
   `calc_infix_to_postfix`
4. **`test_engine.c`** — write tests alongside each section above
5. **`main.c`** — wire everything together last once the pipeline is verified

---

### Bugs Encountered and Fixed

A record of non-obvious mistakes worth remembering.

| Location | Bug | Fix |
|---|---|---|
| `calculator.h` | `calc_ctx_t` used in `OperatorFunc` before it was defined | Added forward declaration above the typedef |
| `calculator.h` | Anonymous `typedef struct {}` couldn't be forward-declared | Added struct tag `struct calc_ctx_t` |
| `engine.c` | All lifecycle and arithmetic functions returned `calc_strerror()` (a `const char*`) instead of a `calc_error_t` | Return enum values directly |
| `engine.c` | `calc_get_operator` returned `calc_strerror()` on both error and not-found paths | Return `NULL` on both paths |
| `engine.c` | `calc_free_result` called `free()` on a stack-allocated struct | Replaced with `memset` reset |
| `parser.c` | `while (input != '\0')` compared a pointer to a character literal | Fixed to `while (*input != '\0')` |
| `parser.c` | `calc_get_operator` called with a `char` instead of `const char*` | Wrap in `char sym[2] = { *input, '\0' }` |
| `parser.c` | `Token.symbol` (a `char[8]`) assigned from a `const char*` | Use `strncpy` |
| `parser.c` | `start_index` computed as `input - input` (always 0) | Saved `base` pointer before the loop |
| `parser.c` | `cur_op->associativity` — field is named `assoc` in the header | Fixed to `cur_op->assoc` |
| `parser.c` | `strtod` used without `#include <stdlib.h>` | Added the include |
| `parser.c` | Unbalanced paren check was inside the pop loop, could exit silently | Moved check to after the loop |

---

### Key Lessons Learned

- A header is a **contract** — only declare what callers need; keep internals in
  the `.c` file.
- Every type must be declared **before** it is used — declaration order matters
  even within a single header. Use forward declarations to break circular
  dependencies rather than reordering everything.
- **Return value = did it succeed; out-parameter = the actual result.** Never
  use the return value for both.
- `typedef double calc_num_t` centralises the numeric type — use it everywhere
  so one change propagates.
- Global mutable state breaks thread safety — bundle state into a context struct
  instead.
- Signal handlers may only write `volatile sig_atomic_t` — everything else is
  undefined behaviour. Set a flag in the handler; act on it in the main loop.
- `sigaction()` is POSIX-only. Use `signal()` for code that must build on
  Windows as well.
- Stale TODOs are worse than no comments — delete a TODO the moment the work is
  done.