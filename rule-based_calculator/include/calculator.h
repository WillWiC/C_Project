#ifndef CALCULATOR_H
#define CALCULATOR_H

/* TODO: Add file description: purpose, public API summary, and thread-safety guarantees
   TODO: Document ownership rules for returned pointers and who frees them */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* --------------------------------------------------------------------------
   Configuration and limits
   -------------------------------------------------------------------------- */
/* TODO: Define MAX_INPUT_LEN and explain how callers should size buffers
   TODO: Define MAX_TOKENS and document behavior when exceeded
   TODO: Add PRECISION or numeric mode (float/double/fixed) and rationale */

/* --------------------------------------------------------------------------
   Error handling
   -------------------------------------------------------------------------- */
/* TODO: Replace plain extern shorts with an enum of error codes:
   // ERR_OK, ERR_DIV_ZERO, ERR_INVALID_TOKEN, ERR_UNBALANCED_PAREN, ERR_OVERFLOW
   TODO: Provide function to convert error codes to human-readable strings
   TODO: Specify whether functions return error codes or set errno-like global */
extern bool DEBUG;
extern enum calc_error_t;    /* TODO: replace with calc_error_t enum */

/* --------------------------------------------------------------------------
   Types and data structures
   -------------------------------------------------------------------------- */
/* TODO: Add TokenType enum (NUMBER, OPERATOR, LPAREN, RPAREN, IDENTIFIER, FUNCTION, END)
   TODO: Add Token struct fields: type, numeric value, start index, length
   TODO: Clarify numeric representation (double vs fixed-point) and document struct semantics
   TODO: Consider opaque context struct (calc_ctx_t) for engine state instead of globals */

typedef struct {
    float decimals;      /* TODO: document meaning or replace with single numeric type */
    long long number;    /* TODO: consider using double for general-purpose calculations */
} number;

/* --------------------------------------------------------------------------
   Arrays and operator metadata
   -------------------------------------------------------------------------- */
/* TODO: Add operator metadata table declaration or accessor:
   // operator symbol; precedence; associativity; function pointer or opcode
   TODO: Document how to extend the operator table with new operators
   TODO: Consider exposing only accessor functions for operator info, not the table itself */

/* --------------------------------------------------------------------------
   Public API function declarations
   -------------------------------------------------------------------------- */
/* TODO: Use a consistent public prefix (e.g., calc_) to avoid symbol collisions
   TODO: Document return values, out-parameters, and thread-safety for each public function
   TODO: Prefer returning error codes and using out-parameters for results */

/* Basic Operations
   TODO: Replace these with clear signatures, e.g., int calc_add(double a, double b, double *out) */
int add();        /* TODO: add parameters and return type semantics */
int substract();  /* TODO: fix spelling to 'subtract' and add parameters */
int multiply();   /* TODO: add parameters and return type semantics */
int divide();     /* TODO: add parameters and document divide-by-zero behavior */

/* Parser Helpers
   TODO: Decide which helpers are public for testing and which remain internal
   TODO: Document ownership of returned token arrays and memory management */

/* Tokenization and scanning */
void whitespace_skipper(const char *input); /* TODO: consider making this static/internal */

/* Token collection and conversion */
void digit_collector();      /* TODO: add parameters (input, token buffer, token count) */
void operator_precedence();  /* TODO: return precedence or accept operator char and return int */
void associativity_checker();/* TODO: return associativity enum or bool for right-assoc */
void conversion_helper();    /* TODO: clarify: infix_to_postfix or parse tree builder */

/* Error utilities */
void error_code();           /* TODO: rename to calc_error_to_string and return const char* */

/* --------------------------------------------------------------------------
   Initialization, lifecycle, and testing hooks
   -------------------------------------------------------------------------- */
/* TODO: Add init_engine/shutdown_engine or context create/destroy functions if stateful
   TODO: Add small, stable test hooks (e.g., token_compare, engine_reset_for_test)
   TODO: Add DEBUG flag accessor function instead of exposing global variable
   TODO: Document logging behavior and how to enable verbose diagnostics */

/* --------------------------------------------------------------------------
   Style, naming, and compatibility notes
   -------------------------------------------------------------------------- */
/* TODO: Add public symbol prefix (e.g., calc_) to avoid name collisions
   TODO: Standardize naming: snake_case for functions, UPPER_SNAKE for macros
   TODO: Describe versioning policy for the header and compatibility guarantees
   TODO: Add cross-reference to README build/run/test instructions
   TODO: Add examples of typical usage: tokenize -> convert_to_rpn -> evaluate */

#endif /* CALCULATOR_H */
