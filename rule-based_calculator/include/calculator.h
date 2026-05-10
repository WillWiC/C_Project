#ifndef CALCULATOR_H
#define CALCULATOR_H

/* 

Calculator Library
Purpose: Provide parsing and evaluation of arithmetic expressions.
Public API: calc_tokenize(), calc_evaluate(), calc_free_result()
Thread-safety: Functions are reentrant if separate contexts are used.

*/

// External libraries

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Configuration and limits

#define MAX_INPUT_LEN 256  // Maximum characters in input expression
#define MAX_TOKENS    50   // Maximum tokens parsed from input
typedef double calc_num_t;  // Numeric type used throughout

// Error handling

extern bool DEBUG;
void calc_set_debug(bool enabled);

typedef enum {
   CALC_ERR_OK = 0,
   CALC_ERR_INVALID_TOKEN,
   CALC_ERR_UNBALANCED_PAREN,
   CALC_ERR_OVERFLOW,
   CALC_ERR_NULL_POINTER,
   CALC_ERR_DIVIDE_BY_ZERO
}  calc_error_t;

// Types and data structures
typedef enum {
   TOKEN_NUMBER,
   TOKEN_OPERATOR,
   TOKEN_LPAREN,
   TOKEN_RPAREN,
   TOKEN_IDENTIFIER,
   TOKEN_FUNCTION,
   TOKEN_END
}  TokenType;

typedef struct {
   TokenType   token;
   double   numeric_value;
   int   start_index;
   int   length;
}  Token;

// Arrays and operator metadata

typedef enum {
   CALC_LEFT,
   CALC_RIGHT,
   CALC_NONE
} Associativity;

typedef calc_error_t (*OperatorFunc)(calc_num_t, calc_num_t, calc_num_t *out);

typedef struct {
   const char *symbol;
   int precedence;  
   Associativity assoc;
   OperatorFunc func;
} Operator;

Operator* calc_get_operator(const char *symbol);

// Public API function declarations

// returns error code for each operation
calc_error_t calc_add(double num_1, double num_2, double *out);
calc_error_t calc_subtract(double num_1, double num_2, double *out);
calc_error_t calc_multiply(double num_1, double num_2, double *out);
calc_error_t calc_divide(double num_1, double num_2, double *out);

// Tokenization
int tokenize(char* input, Token* token);  // Tokenize the input
void whitespace_skipper(const char *input);  // Advance past whitespace in input

// Token Collection and Output
char* digit_collector(char* input, Token* token_buffer, int token_count);  // Store digits from the input
int operator_precedence(double num_1, double num_2, Operator* op);   // Create postfix
Associativity associativity_checker(Associativity output_associativity);  // Return associativity of an operator
calc_error_t calc_infix_to_postfix(const Token *in, size_t in_count, Token *out, size_t *out_count);  // convert infix to postfix expression
calc_error_t calc_evaluate(const Token *tokens, size_t token_count, calc_num_t *result);  // Evaluate postfix expression and return result

// Error Utility 
const char* calc_strerror(calc_error_t err); // Convert enum to string

// Initialization, lifecycle, and testing hooks

/* 
TODO: Add init_engine/shutdown_engine or context create/destroy functions if stateful
TODO: Add small, stable test hooks (e.g., token_compare, engine_reset_for_test)
TODO: Add DEBUG flag accessor function instead of exposing global variable
TODO: Document logging behavior and how to enable verbose diagnostics
*/



#endif