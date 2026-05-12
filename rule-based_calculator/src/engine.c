/* ==========================================================================
   engine.c — Implementation TODO List
   Complete in order; each section depends on the one above it.
   ========================================================================== 
*/

/* --------------------------------------------------------------------------
   6. TEST HOOKS
   Depends on: 1.1
   --------------------------------------------------------------------------

   TODO 6.1 — calc_ctx_reset_for_test(calc_ctx_t *ctx)
      - Return if ctx is NULL
      - Reset error_state to CALC_ERR_OK
      - Clear token_buffer
      - Do NOT clear operator_table — operator table should survive resets
      - Faster than a full calc_shutdown / calc_init cycle between tests
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <calculator.h>

// error utility
const char* calc_strerror(calc_error_t error){
   switch (error) {
      case CALC_ERR_OK:              return "No error";
      case CALC_ERR_INVALID_TOKEN:   return "Invalid token";
      case CALC_ERR_UNBALANCED_PAREN:return "Unbalanced parenthesis";
      case CALC_ERR_OVERFLOW:        return "Numeric overflow";
      case CALC_ERR_NULL_POINTER:    return "Null Pointer";
      case CALC_ERR_DIVIDE_BY_ZERO:   return "Cannot divide by zero";
      default:                  return "Unknown error";
   }
}

// lifecycle
calc_error_t calc_init(calc_ctx_t* ctx) {
   if (ctx == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }
   memset(ctx, 0, sizeof(*ctx));
   ctx->error_state = CALC_ERR_OK;
   ctx->debug = false;
   static const Operator ops[] ={
    { "+", 1, CALC_LEFT, calc_add      },
    { "-", 1, CALC_LEFT, calc_subtract },
    { "*", 2, CALC_LEFT, calc_multiply },
    { "/", 2, CALC_LEFT, calc_divide   },
   };

   memcpy(ctx->operator_table, ops, sizeof(ctx->operator_table));

   return calc_strerror(CALC_ERR_OK);
}

calc_error_t calc_shutdown(calc_ctx_t* ctx) {
   if (ctx == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }

   memset(ctx, 0, sizeof(*ctx));
   return calc_strerror(CALC_ERR_OK);
}

int calc_set_debug(calc_ctx_t *ctx, bool debug){
   if (ctx == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }

   ctx->debug = debug;
   return calc_strerror(CALC_ERR_OK);
}

int calc_free_result(calc_ctx_t *ctx){
   if (ctx == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }

   free(ctx);
   return calc_strerror(CALC_ERR_OK);
}


// arithmetric operations
calc_error_t calc_add(calc_ctx_t *ctx, calc_num_t a, calc_num_t b, calc_num_t *out) {
   if (ctx == NULL || out == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }
   *out = a + b;

   return calc_strerror(CALC_ERR_OK);
}

calc_error_t calc_subtract(calc_ctx_t *ctx, calc_num_t a, calc_num_t b, calc_num_t *out) {
   if (ctx == NULL || out == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }
   *out = a - b;

   return calc_strerror(CALC_ERR_OK);
}

calc_error_t calc_multiply(calc_ctx_t *ctx, calc_num_t a, calc_num_t b, calc_num_t *out) {
   if (ctx == NULL || out == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }
   *out = a * b;

   if (isinf(*out)) {
      return calc_strerror(CALC_ERR_OVERFLOW);
   }

   return calc_strerror(CALC_ERR_OK);
}

calc_error_t calc_divide(calc_ctx_t *ctx, calc_num_t a, calc_num_t b, calc_num_t *out) {
   if (ctx == NULL || out == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }

   if (b == 0){
      return calc_strerror(CALC_ERR_DIVIDE_BY_ZERO);
   }

   *out = a / b;

   return calc_strerror(CALC_ERR_OK);
}

Operator *calc_get_operator(calc_ctx_t *ctx, const char*symbol) {
   if (ctx == NULL || symbol == NULL) {
      return calc_strerror(CALC_ERR_NULL_POINTER);
   }
   for (int i = 0; i < OPERATOR_TABLE_SIZE; i++) {
         if (strcmp(ctx->operator_table[i].symbol, symbol) == 0) {
               return &ctx->operator_table[i];
         }
   }

   return calc_strerror(CALC_ERR_OK);
}

calc_error_t calc_evaluate(calc_ctx_t *ctx, const Token *tokens, size_t token_count, calc_num_t *result) {
    if (!ctx || !tokens || !result)
        return CALC_ERR_NULL_POINTER;

    calc_num_t stack[MAX_TOKENS];
    int stack_top = 0;

    for (size_t i = 0; i < token_count; i++) {
        if (tokens[i].token_type == TOKEN_END) {
            break;
        } 
        else if (tokens[i].token_type == TOKEN_NUMBER) {
            if (stack_top >= MAX_TOKENS) return CALC_ERR_INVALID_TOKEN;
            stack[stack_top++] = tokens[i].numeric_value;
        } 
        else if (tokens[i].token_type == TOKEN_OPERATOR) {
            if (stack_top < 2) return CALC_ERR_INVALID_TOKEN;
            calc_num_t b = stack[--stack_top];
            calc_num_t a = stack[--stack_top];
            const Operator *op = calc_get_operator(ctx, tokens[i].symbol);
            if (!op) return CALC_ERR_INVALID_TOKEN;
            calc_error_t err = op->func(ctx, a, b, &stack[stack_top]);
            if (err != CALC_ERR_OK) return err;
            stack_top++;
        }
    }

    if (stack_top < 1) return CALC_ERR_INVALID_TOKEN;
    *result = stack[--stack_top];

    if (isinf(*result)) return CALC_ERR_OVERFLOW;
    return CALC_ERR_OK;
}

void calc_ctx_reset_for_test(calc_ctx_t *ctx) {
    if (ctx == NULL) return;

    ctx->error_state = CALC_ERR_OK;

    // operator_table is intentionally NOT cleared — survives resets by design
    memset(ctx->token_buffer, 0, sizeof(ctx->token_buffer[0]) * MAX_TOKENS);
}