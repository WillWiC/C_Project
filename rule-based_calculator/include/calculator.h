#ifndef CALCULATOR_H
#define CALCULATOR_H

/*
Calculator Library — Header
Purpose: Provide declarations for parsing and evaluation of arithmetic expressions
Public API: calc_tokenize(), calc_evaluate(), calc_free_result()
Thread-safety: Functions are reentrant when called with separate calc_ctx_t instances
*/

// External libraries

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Configuration and Declarations

#define MAX_INPUT_LEN 256  // Maximum characters in input expression
#define MAX_TOKENS    50   // Maximum tokens parsed from input
#define OPERATOR_TABLE_SIZE 4
typedef double calc_num_t;  // Numeric type used throughout

typedef enum {
   CALC_ERR_OK = 0,
   CALC_ERR_INVALID_TOKEN,
   CALC_ERR_UNBALANCED_PAREN,
   CALC_ERR_OVERFLOW,
   CALC_ERR_NULL_POINTER,
   CALC_ERR_DIVIDE_BY_ZERO
}  calc_error_t;

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
   TokenType   token_type;
   char        symbol[8];
   double      numeric_value;
   int         start_index;
   int         length;
}  Token;

typedef enum {
   CALC_LEFT,
   CALC_RIGHT,
   CALC_NONE
} Associativity;

typedef calc_error_t (*OperatorFunc)(calc_ctx_t *ctx, calc_num_t a, calc_num_t b, calc_num_t *out);

typedef struct {
   const char *symbol;
   int precedence;  
   Associativity assoc;
   OperatorFunc func;
} Operator;

typedef struct {
      Operator operator_table[MAX_TOKENS];
      calc_error_t error_state;
      Token token_buffer[MAX_TOKENS];
      bool debug;
} calc_ctx_t;

Operator* calc_get_operator(calc_ctx_t *ctx, const char *symbol);

// Public API function declarations

// returns error code for each operation
calc_error_t calc_add(calc_ctx_t* calc_status,calc_num_t num_1, calc_num_t num_2, calc_num_t *out);
calc_error_t calc_subtract(calc_ctx_t* calc_status,calc_num_t num_1, calc_num_t num_2, calc_num_t *out);
calc_error_t calc_multiply(calc_ctx_t* calc_status, calc_num_t num_1, calc_num_t num_2, calc_num_t *out);
calc_error_t calc_divide(calc_ctx_t* calc_status, calc_num_t num_1, calc_num_t num_2, calc_num_t *out);

// Tokenization
calc_error_t calc_tokenize(calc_ctx_t* calc_status, char* input, Token* token, size_t *token_count);  // Tokenize the input
const char* whitespace_skipper(calc_ctx_t* calc_status, const char *input);  // Advance past whitespace in input

// Token Collection and Output
char* digit_collector(calc_ctx_t* calc_status, char* input, Token* token_buffer, int token_count);  // Store digits from the input
int operator_precedence(calc_ctx_t* calc_status, const Operator *op);  // Return precedence level of an operator
Associativity associativity_checker(calc_ctx_t* calc_status, Associativity output_associativity);  // Return associativity of an operator
calc_error_t calc_infix_to_postfix(calc_ctx_t* calc_status, const Token *in, size_t in_count, Token* token, size_t *out_count);  // convert infix to postfix expression
calc_error_t calc_evaluate(calc_ctx_t* calc_status, const Token *tokens, size_t token_count, calc_num_t *result);  // Evaluate postfix expression and return result

// Error Utility 
const char* calc_strerror(calc_error_t err); // Convert enum to string

// Initialization, lifecycle, thread-safety, and testing hooks

calc_error_t calc_init(calc_ctx_t*);
calc_error_t calc_shutdown(calc_ctx_t*);
int calc_free_result(calc_ctx_t*);

int token_compare(Token* token_1, Token* token_2);
int calc_set_debug(calc_ctx_t*, bool);
void calc_ctx_reset_for_test(calc_ctx_t*);

#endif