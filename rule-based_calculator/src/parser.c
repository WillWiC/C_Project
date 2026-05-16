#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <calculator.h>

const char *whitespace_skipper(calc_ctx_t *ctx, const char *input) {
    if (ctx == NULL || input == NULL) {
        return NULL;
    }
    while (*input != '\0' && isspace((unsigned char)*input)) {
        input++;
    }
    return input;
}


char *digit_collector(calc_ctx_t *ctx, char *input,
                      Token *token_buffer, int token_count) {
    if (ctx == NULL || input == NULL || token_buffer == NULL) {
        return NULL;
    }

    char *end;
    calc_num_t value = strtod(input, &end);

    token_buffer[token_count].token_type    = TOKEN_NUMBER;
    token_buffer[token_count].numeric_value = value;
    token_buffer[token_count].start_index   = 0;
    token_buffer[token_count].length        = (int)(end - input);

    return end;
}

int operator_precedence(calc_ctx_t *ctx, const Operator *op) {
    if (ctx == NULL || op == NULL) {
        return -1;
    }
    return op->precedence;
}


Associativity associativity_checker(calc_ctx_t *ctx,
                                    Associativity output_associativity) {
    if (ctx == NULL) {
        return CALC_NONE;
    }
    return output_associativity;
}

calc_error_t calc_tokenize(calc_ctx_t *ctx, char *input,
                           Token *token, size_t *token_count) {
    if (ctx == NULL || input == NULL || token == NULL || token_count == NULL) {
        return CALC_ERR_NULL_POINTER;
    }

    *token_count = 0;
    const char *base = input;   /* remember start for absolute offsets */

    while (*input != '\0') {
        input = (char *)whitespace_skipper(ctx, input);
        if (input == NULL) {
            return CALC_ERR_NULL_POINTER;
        }
        if (*input == '\0') {
            break;
        }

        /* --- Number token --- */
        if (isdigit((unsigned char)*input) || *input == '.') {
            char *after = digit_collector(ctx, input, token, (int)*token_count);
            if (after == NULL) {
                return CALC_ERR_NULL_POINTER;
            }
            token[*token_count].start_index = (int)(input - base);
            (*token_count)++;
            input = after;
        }

        /* --- Operator token --- */
        else if (*input == '+' || *input == '-' ||
                 *input == '*' || *input == '/') {

            /* calc_get_operator needs a null-terminated string */
            char sym[2] = { *input, '\0' };
            const Operator *op = calc_get_operator(ctx, sym);

            token[*token_count].token_type  = TOKEN_OPERATOR;
            token[*token_count].start_index = (int)(input - base);
            token[*token_count].length      = 1;

            if (op == NULL) {
                token[*token_count].symbol[0] = *input;
                token[*token_count].symbol[1] = '\0';
                ctx->error_state = CALC_ERR_INVALID_TOKEN;
            } else {
                /* symbol is char[8]; copy from the Operator's const char* */
                strncpy(token[*token_count].symbol, op->symbol,
                        sizeof(token[*token_count].symbol) - 1);
                token[*token_count].symbol[sizeof(token[*token_count].symbol) - 1] = '\0';
            }

            (*token_count)++;
            input++;
        }

        /* --- Left parenthesis --- */
        else if (*input == '(') {
            token[*token_count].token_type  = TOKEN_LPAREN;
            token[*token_count].start_index = (int)(input - base);
            token[*token_count].length      = 1;
            (*token_count)++;
            input++;
        }

        /* --- Right parenthesis --- */
        else if (*input == ')') {
            token[*token_count].token_type  = TOKEN_RPAREN;
            token[*token_count].start_index = (int)(input - base);
            token[*token_count].length      = 1;
            (*token_count)++;
            input++;
        }

        /* --- Unrecognised character --- */
        else {
            ctx->error_state = CALC_ERR_INVALID_TOKEN;
            return CALC_ERR_INVALID_TOKEN;
        }

        if (*token_count >= MAX_TOKENS) {
            return CALC_ERR_OVERFLOW;
        }
    }

    token[*token_count].token_type = TOKEN_END;
    return CALC_ERR_OK;
}

/* --------------------------------------------------------------------------
   INFIX → POSTFIX  (Shunting-Yard Algorithm)
   -------------------------------------------------------------------------- */
calc_error_t calc_infix_to_postfix(calc_ctx_t *ctx,
                                   const Token *in,  size_t  in_count,
                                   Token       *out, size_t *out_count) {
    if (ctx == NULL || in == NULL || out == NULL || out_count == NULL) {
        return CALC_ERR_NULL_POINTER;
    }

    *out_count = 0;

    Token output_queue[MAX_TOKENS];
    Token operator_stack[MAX_TOKENS];
    int   stack_top = -1;

    for (size_t i = 0; i < in_count; i++) {
        const Token *tok = &in[i];

        switch (tok->token_type) {

        case TOKEN_NUMBER:
            output_queue[*out_count] = *tok;
            (*out_count)++;
            break;

        case TOKEN_OPERATOR: {
            const Operator *cur_op = calc_get_operator(ctx, tok->symbol);
            if (cur_op == NULL) {
                return CALC_ERR_INVALID_TOKEN;
            }
            int cur_prec        = operator_precedence(ctx, cur_op);
            /* Bug fixed: field is 'assoc' not 'associativity' */
            Associativity cur_assoc = associativity_checker(ctx, cur_op->assoc);

            while (stack_top >= 0 &&
                   operator_stack[stack_top].token_type == TOKEN_OPERATOR) {
                const Operator *top_op =
                    calc_get_operator(ctx, operator_stack[stack_top].symbol);
                if (top_op == NULL) break;
                int top_prec = operator_precedence(ctx, top_op);

                if (top_prec > cur_prec ||
                    (top_prec == cur_prec && cur_assoc == CALC_LEFT)) {
                    output_queue[*out_count] = operator_stack[stack_top];
                    (*out_count)++;
                    stack_top--;
                } else {
                    break;
                }
            }

            operator_stack[++stack_top] = *tok;
            break;
        }

        case TOKEN_LPAREN:
            operator_stack[++stack_top] = *tok;
            break;

        case TOKEN_RPAREN:
            /* Pop until matching '(' found or stack exhausted */
            while (stack_top >= 0 &&
                   operator_stack[stack_top].token_type != TOKEN_LPAREN) {
                output_queue[*out_count] = operator_stack[stack_top];
                (*out_count)++;
                stack_top--;
            }
            /* Bug fixed: check AFTER the loop, not inside it */
            if (stack_top < 0) {
                return CALC_ERR_UNBALANCED_PAREN;
            }
            /* Discard the TOKEN_LPAREN */
            stack_top--;
            break;

        case TOKEN_END:
            goto drain_stack;

        default:
            break;
        }
    }

drain_stack:
    while (stack_top >= 0) {
        if (operator_stack[stack_top].token_type == TOKEN_LPAREN) {
            return CALC_ERR_UNBALANCED_PAREN;
        }
        output_queue[*out_count] = operator_stack[stack_top];
        (*out_count)++;
        stack_top--;
    }

    memcpy(out, output_queue, (*out_count) * sizeof(Token));
    return CALC_ERR_OK;
}