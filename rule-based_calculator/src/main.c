#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <calculator.h>

/* --------------------------------------------------------------------------
   SERVICE STATE
   -------------------------------------------------------------------------- */

/* Volatile so the compiler doesn't optimise away reads in the signal handler.
   sig_atomic_t is guaranteed safe to read/write from a signal context.      */
static volatile sig_atomic_t g_running = 1;

/* --------------------------------------------------------------------------
   SIGNAL HANDLERS
   -------------------------------------------------------------------------- */

static void handle_shutdown(int signum) {
    (void)signum;   /* suppress unused-parameter warning */
    g_running = 0;
}

static void install_signal_handlers(void) {
    signal(SIGINT,  handle_shutdown);   /* Ctrl-C            */
    signal(SIGTERM, handle_shutdown);   /* kill / taskkill   */
}

/* --------------------------------------------------------------------------
   Main UI
   -------------------------------------------------------------------------- */

static void print_banner(void) {
    puts("=========================================");
    puts("  Basic Operation Calculator v1.0   ");
    puts("=========================================");
    puts("  Supported operators: + - * /           ");
    puts("  Parentheses:         ( )               ");
    puts("  Commands:  'help'  'quit'  Ctrl-C      ");
    puts("=========================================");
}

static void print_help(void) {
    puts("");
    puts("Usage:");
    puts("  Type an arithmetic expression and press Enter.");
    puts("  Examples:");
    puts("    > 1 + 2");
    puts("    > (3 + 4) * 2");
    puts("    > 10 / (5 - 3)");
    puts("");
    puts("Commands:");
    puts("  help     show this message");
    puts("  quit     exit the service");
    puts("  Ctrl-C   exit the service");
    puts("");
}

/* --------------------------------------------------------------------------
   EVALUATE ONE EXPRESSION
   Returns CALC_ERR_OK on success; prints the error and returns the code
   on failure.  The result is printed here on success.
   -------------------------------------------------------------------------- */

static calc_error_t evaluate_expression(calc_ctx_t *ctx, char *input) {
    Token       tokens[MAX_TOKENS];
    Token       postfix[MAX_TOKENS];
    size_t      token_count = 0;
    size_t      postfix_count = 0;
    calc_num_t  result = 0.0;
    calc_error_t err;

    /* Reset per-expression context state (clears error_state, token_buffer) */
    calc_ctx_reset_for_test(ctx);

    /* --- Step 1: tokenize --- */
    err = calc_tokenize(ctx, input, tokens, &token_count);
    if (err != CALC_ERR_OK) {
        fprintf(stderr, "  Tokenize error: %s\n", calc_strerror(err));
        return err;
    }

    /* Empty expression — nothing to evaluate */
    if (token_count == 0 ||
        (token_count == 1 && tokens[0].token_type == TOKEN_END)) {
        return CALC_ERR_OK;
    }

    /* --- Step 2: infix → postfix (Shunting-Yard) --- */
    err = calc_infix_to_postfix(ctx, tokens, token_count, postfix, &postfix_count);
    if (err != CALC_ERR_OK) {
        fprintf(stderr, "  Parse error: %s\n", calc_strerror(err));
        return err;
    }

    /* --- Step 3: evaluate postfix --- */
    err = calc_evaluate(ctx, postfix, postfix_count, &result);
    if (err != CALC_ERR_OK) {
        fprintf(stderr, "  Evaluation error: %s\n", calc_strerror(err));
        return err;
    }

    /* --- Step 4: print result --- */
    /* Use %g to suppress trailing zeros (3.0 → "3", 3.14 → "3.14") */
    printf("  = %g\n", result);
    return CALC_ERR_OK;
}

/* --------------------------------------------------------------------------
   MAIN — service entry point
   -------------------------------------------------------------------------- */

int main(void) {
    calc_ctx_t ctx;
    char       input[MAX_INPUT_LEN];

    /* --- Initialise --- */
    install_signal_handlers();

    if (calc_init(&ctx) != CALC_ERR_OK) {
        fprintf(stderr, "Fatal: failed to initialise calculator context.\n");
        return EXIT_FAILURE;
    }

    print_banner();

    /* --- REPL loop --- */
    while (g_running) {
        printf("\n> ");
        fflush(stdout);

        /* fgets returns NULL on EOF (Ctrl-D) or if a signal interrupted it */
        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* EOF or signal — exit cleanly */
            break;
        }

        /* Strip trailing newline */
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[--len] = '\0';
        }

        /* Skip blank lines */
        if (len == 0) {
            continue;
        }

        /* --- Built-in commands --- */
        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            g_running = 0;
            continue;
        }

        if (strcmp(input, "help") == 0) {
            print_help();
            continue;
        }

        /* --- Evaluate --- */
        evaluate_expression(&ctx, input);
    }

    /* --- Graceful shutdown --- */
    puts("\nShutting down calculator service. Goodbye.");
    calc_shutdown(&ctx);

    return EXIT_SUCCESS;
}