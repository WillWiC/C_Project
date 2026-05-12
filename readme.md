# Building C Projects

---

## 0. C Tutorial
**Focus:** Basic tutorials to familiarise with the syntax and key concepts of C

---

## 1. Basic Calculator Service
**Focus:** Basics (Variables, I/O, Control Flow) → Library Design, Header Files, Memory Layout

### Key Concepts
- **Data Types:** `double` / `calc_num_t` for numeric precision; `typedef` to centralise the numeric type.
- **Input/Output:** `printf()` for display; `scanf()` for user input.
- **Control Flow:** `switch` for operator dispatch; `if-else` for error guards.
- **Header Files:** Declarations vs definitions; include guards; declaration order.
- **Error Handling:** Enum-based error codes (`calc_error_t`); out-parameter pattern; `calc_strerror()` for display.
- **Function Pointers:** `OperatorFunc` typedef; storing behaviour inside a struct (`Operator`).
- **Context Pattern:** `calc_ctx_t` bundles all engine state; passed as first parameter to every function for thread safety.
- **Memory:** `memset` for zero-initialisation; fixed-size arrays inside structs; no heap allocation in current design.

### Project Structure
```text
project-root/
├── include/
│   └── calculator.h    # All declarations — types, enums, structs, prototypes
├── src/
│   ├── main.c          # Entry point — wires together parser and engine
│   ├── parser.c        # Tokenisation, infix-to-postfix conversion
│   └── engine.c        # Arithmetic, operator table, evaluation, lifecycle
├── tests/
│   └── test_engine.c   # Unit tests using test hooks
├── Makefile
└── README.md
```

### Public API
| Function | Purpose |
|---|---|
| `calc_init(ctx)` | Initialise context and operator table |
| `calc_shutdown(ctx)` | Clean up context |
| `calc_tokenize(ctx, input, tokens, count)` | Lex raw input into token array |
| `calc_evaluate(ctx, tokens, count, result)` | Evaluate postfix token array |
| `calc_free_result(ctx)` | Free any library-allocated result state |
| `calc_strerror(err)` | Convert error code to string for display |

### Implementation Order
1. **`engine.c`** — `calc_init`, `calc_shutdown`, arithmetic functions, `calc_evaluate`
2. **`parser.c`** — `calc_tokenize`, `whitespace_skipper`, `digit_collector`, `calc_infix_to_postfix`
3. **`test_engine.c`** — write tests alongside each section above
4. **`main.c`** — wire everything together last

### Key Lessons Learned
- A header is a **contract** — only declare what callers need; keep internals in the `.c` file.
- Every type must be declared **before** it is used — declaration order matters even in headers.
- **Return value = did it succeed; out-parameter = the actual result.** Never use the return value for both.
- `typedef double calc_num_t` centralises the numeric type — use it everywhere so one change propagates.
- Global mutable state breaks thread safety — bundle state into a context struct instead.
- Stale TODOs are worse than no comments — delete a TODO the moment the work is done.

---