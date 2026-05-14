/* ==========================================================================
   parser.c — Implementation TODO List
   Complete in order; each section depends on the one above it.
   ========================================================================== */

/* --------------------------------------------------------------------------
   4. TOKENIZER
   Depends on: 1.1 (whitespace_skipper), 2.1 (digit_collector)
   --------------------------------------------------------------------------

   TODO 4.1 — calc_tokenize(calc_ctx_t *ctx, char *input,
                             Token *token, size_t *token_count)
      - Return CALC_ERR_NULL_POINTER if ctx, input, token, or
        token_count is NULL
      - Set *token_count = 0 at the start
      - Walk through input character by character:

            while current char is not '\0':
               skip whitespace using whitespace_skipper()

               if isdigit() or '.' :
                  call digit_collector() to read the number token
                  increment *token_count
                  advance input pointer to where digit_collector stopped

               else if current char is an operator (+, -, *, /):
                  look up operator with calc_get_operator()
                  if not found: store TOKEN_OPERATOR token with no func,
                                set ctx->error_state = CALC_ERR_INVALID_TOKEN
                  write TOKEN_OPERATOR token into buffer:
                        token[*token_count].token       = TOKEN_OPERATOR
                        token[*token_count].start_index = current position
                        token[*token_count].length      = 1
                  increment *token_count
                  advance input pointer by 1

               else if current char is '(':
                  write TOKEN_LPAREN token
                  increment *token_count, advance pointer

               else if current char is ')':
                  write TOKEN_RPAREN token
                  increment *token_count, advance pointer

               else:
                  set ctx->error_state = CALC_ERR_INVALID_TOKEN
                  return CALC_ERR_INVALID_TOKEN

               if *token_count >= MAX_TOKENS:
                  return CALC_ERR_OVERFLOW

      - Write a TOKEN_END token as the final entry
      - Return CALC_ERR_OK on success
*/

/* --------------------------------------------------------------------------
   5. INFIX TO POSTFIX (SHUNTING-YARD ALGORITHM)
   Depends on: 3.1 (operator_precedence), 3.2 (associativity_checker),
               4.1 (calc_tokenize produces the input token array)
   This is the most complex function in parser.c — implement last.
   --------------------------------------------------------------------------

   TODO 5.1 — calc_infix_to_postfix(calc_ctx_t *ctx,
                                     const Token *in, size_t in_count,
                                     Token *out, size_t *out_count)
      - Return CALC_ERR_NULL_POINTER if any pointer is NULL
      - Set *out_count = 0
      - Set up two local structures:
            Token output_queue[MAX_TOKENS]   — the postfix result
            Token operator_stack[MAX_TOKENS] — temporary operator storage
            int stack_top = -1               — stack pointer

      - Walk through each token in the input array:

            if TOKEN_NUMBER:
               write directly to output_queue[*out_count]
               increment *out_count

            if TOKEN_OPERATOR:
               look up the operator with calc_get_operator()
               while stack is not empty
               AND top of stack is TOKEN_OPERATOR
               AND (top precedence > current precedence
                    OR (top precedence == current precedence
                        AND associativity is CALC_LEFT)):
                  pop top of stack to output_queue
                  increment *out_count
               push current token onto operator_stack
               increment stack_top

            if TOKEN_LPAREN:
               push onto operator_stack
               increment stack_top

            if TOKEN_RPAREN:
               while top of stack is not TOKEN_LPAREN:
                  pop top of stack to output_queue
                  increment *out_count
                  if stack is empty: return CALC_ERR_UNBALANCED_PAREN
               pop and discard the TOKEN_LPAREN

            if TOKEN_END:
               break out of loop

      - After the loop, pop all remaining operators from stack to output:
            while stack_top >= 0:
               if top is TOKEN_LPAREN: return CALC_ERR_UNBALANCED_PAREN
               pop to output_queue
               increment *out_count

      - Copy output_queue into out
      - Return CALC_ERR_OK
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <calculator.h>

const char* whitespace_skipper(calc_ctx_t *ctx, const char *input){

   if (ctx== NULL || input == NULL) {
      return NULL;
   }

   char *read_input = input;
   char *nws_input = input;
   while (*read_input != '\0') {
      if (!isspace((unsigned char) *read_input)) {
         *nws_input = *read_input;
         nws_input++;
      }

      read_input++;
      *nws_input = '\0';

   }

   return nws_input;
}

char* digit_collector(calc_ctx_t* ctx, char* input, Token* token_buffer, int token_count) {

   if (ctx == NULL || input == NULL || token_buffer == NULL)
      return NULL;

   char *start_checker = input;
   char *read_input = input;
   while (*read_input != '\0' && (isdigit((unsigned char)*read_input) || *read_input == '.')) {
      read_input++;
   }

   char *end_checker;
   calc_num_t value = strtod(start_checker, &end_checker);

   token_buffer[token_count].token_type      = TOKEN_NUMBER;
   token_buffer[token_count].numeric_value   = value;
   token_buffer[token_count].start_index     = start_checker - input;
   token_buffer[token_count].length          = (int)(read_input - start_checker);

   return read_input;
}

/* --------------------------------------------------------------------------
   3. OPERATOR PRECEDENCE AND ASSOCIATIVITY
   Depends on: engine — calc_get_operator() must be working
   Implement these before calc_infix_to_postfix which calls both.
   --------------------------------------------------------------------------

   TODO 3.1 — operator_precedence(calc_ctx_t *ctx, const Operator *op)
      - Return -1 if ctx or op is NULL (signals unknown/invalid)
      - Return op->precedence directly
      - No calculation needed — precedence is already stored in the struct

   TODO 3.2 — associativity_checker(calc_ctx_t *ctx,
                                     Associativity output_associativity)
      - Return CALC_NONE if ctx is NULL
      - Return output_associativity directly
      - Used by calc_infix_to_postfix to decide whether to pop operators
        from the stack before pushing the current one
*/

