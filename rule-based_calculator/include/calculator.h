#ifndef CALCULATOR_H
#define CALCULATOR_H

/* 
--------------------------------------------------------------------------
Calculator Library
Purpose: Provide parsing and evaluation of arithmetic expressions.
Public API: tokenize(), evaluate(), free_result()
Thread-safety: Functions are reentrant if separate contexts are used.
--------------------------------------------------------------------------
*/

/*
--------------------------------------------------------------------------
External libraries
--------------------------------------------------------------------------
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* 
--------------------------------------------------------------------------
   Configuration and limits
-------------------------------------------------------------------------- 
*/

#define MAX_INPUT_LEN 256;  // Maximum characters in input expression
#define MAX_TOKENS    50;   // Maximum tokens parsed from input
typedef double calc_num_t;  // Numeric type used throughout

/* 
--------------------------------------------------------------------------
   Error handling
-------------------------------------------------------------------------- 
*/

extern bool DEBUG;

typedef enum {
   ERR_OK = 0,
   ERR_INVALID_TOKEN,
   ERR_UNBALANCED_PAREN,
   ERR_OVERFLOW,
   ERR_NULL_POINTER,
   ERR_DIVIDE_BY_ZERO
}  calc_error_t;

static const char* error_code(calc_error_t error) {
   switch (error) {
      case ERR_OK:              return "No error";
      case ERR_INVALID_TOKEN:   return "Invalid token";
      case ERR_UNBALANCED_PAREN:return "Unbalanced parenthesis";
      case ERR_OVERFLOW:        return "Numeric overflow";
      case ERR_NULL_POINTER:    return "Null Pointer";
      case ERR_DIVIDE_BY_ZERO:   return "Cannot divide by zero";
      default:                  return "Unknown error";
   }
}

/* 
--------------------------------------------------------------------------
   Types and data structures
-------------------------------------------------------------------------- 
*/

/* 
TODO: Add TokenType enum (NUMBER, OPERATOR, LPAREN, RPAREN, IDENTIFIER, FUNCTION, END)
TODO: Add Token struct fields: type, numeric value, start index, length
TODO: Clarify numeric representation (double vs fixed-point) and document struct semantics
TODO: Consider opaque context struct (calc_ctx_t) for engine state instead of globals 
*/

typedef enum {
   NUMBER,
   OPERATOR,
   LPAREN,
   RPAREN,
   IDENTIFIER,
   FUNCTION,
   END
}  TokenType;

typedef struct {
   char* type;
   double numeric_value;
   int start_index;
   int length;
}  Token;

/*
--------------------------------------------------------------------------
   Arrays and operator metadata
--------------------------------------------------------------------------
*/

/*
TODO: Add operator metadata table declaration or accessor:
      // operator symbol; precedence; associativity; function pointer or opcode
TODO: Document how to extend the operator table with new operators
TODO: Consider exposing only accessor functions for operator info, not the table itself 
*/

/* 
--------------------------------------------------------------------------
   Public API function declarations
-------------------------------------------------------------------------- 
*/

/* TODO: Use a consistent public prefix (e.g., calc_) to avoid symbol collisions
   TODO: Document return values, out-parameters, and thread-safety for each public function
   TODO: Prefer returning error codes and using out-parameters for results */

// Basic Operations

int calc_add(double num_1, double num_2, double *out) {
   if (out == NULL) return error_code(ERR_NULL_POINTER);
   *out = num_1 + num_2;
   return error_code(ERR_OK);
}

int calc_subtract(double num_1, double num_2, double *out){
   if (out == NULL) return error_code(ERR_NULL_POINTER);
   *out = num_1 - num_2;
   return error_code(ERR_OK);
}

int calc_multiply(double num_1, double num_2, double *out) {
   if (out == NULL) return error_code(ERR_NULL_POINTER);
   *out = num_1 * num_2;
   return error_code(ERR_OK);
}

int calc_divide(double num_1, double num_2, double *out) {
   if (num_2 == 0) {
      return error_code(ERR_DIVIDE_BY_ZERO);
   } 
   else{
   *out = num_1 / num_2;
   return error_code(ERR_OK);
   }
}

/* Parser Helpers
   TODO: Decide which helpers are public for testing and which remain internal
   TODO: Document ownership of returned token arrays and memory management */

/* Tokenization and scanning */
static void whitespace_skipper(const char *input); /* TODO: consider making this static/internal */

/* Token collection and conversion */
void digit_collector();      /* TODO: add parameters (input, token buffer, token count) */
void operator_precedence();  /* TODO: return precedence or accept operator char and return int */
void associativity_checker();/* TODO: return associativity enum or bool for right-assoc */
void conversion_helper();    /* TODO: clarify: infix_to_postfix or parse tree builder */

/* Error utilities */
void calc_error_to_string(const char*);

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

#endif
