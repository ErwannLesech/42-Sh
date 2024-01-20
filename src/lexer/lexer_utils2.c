#include "lexer.h"

void print_token(struct token token)
{
    char *tokens[] = {
        // Step1
        [TOKEN_IF] = "TOKEN_IF",
        [TOKEN_THEN] = "TOKEN_THEN",
        [TOKEN_ELIF] = "TOKEN_ELIF",
        [TOKEN_ELSE] = "TOKEN_ELSE",
        [TOKEN_FI] = "TOKEN_FI",
        [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
        [TOKEN_WORD] = "TOKEN_WORD",
        [TOKEN_EOL] = "TOKEN_EOL",
        [TOKEN_EOF] = "TOKEN_EOF",
        [TOKEN_ERROR] = "TOKEN_ERROR",
        // Step 2
        [TOKEN_DONE] = "TOKEN_DONE",
        [TOKEN_AND] = "TOKEN_AND",
        [TOKEN_OR] = "TOKEN_OR",
        [TOKEN_PIPE] = "TOKEN_PIPE",
        [TOKEN_NEGATE] = "TOKEN_NEGATE",
        [TOKEN_IONUMBER] = "TOKEN_IONUMBER",
        [TOKEN_REDIR] = "TOKEN_REDIR",
        [TOKEN_DOUBLE_QUOTE] = "TOKEN_DOUBLE_QUOTE",
        [TOKEN_WORD_DOUBLE_QUOTE] = "TOKEN_WORD_DOUBLE_QUOTE",
        [TOKEN_WORD_ASSIGNMENT] = "TOKEN_WORD_ASSIGNMENT",
        [TOKEN_VARIABLE] = "TOKEN_VARIABLE",

        // Internal values for lexer
        [TOKEN_VARIABLE_VALUE] = "TOKEN_VARIABLE_VALUE",
        [TOKEN_VARIABLE_AND_DOUBLE_QUOTE] = "TOKEN_VARIABLE_AND_DOUBLE_QUOTE"
    };
    printf("Token: %s\n", tokens[token.type]);
}