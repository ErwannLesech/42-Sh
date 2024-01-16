#include <ctype.h>
#include "parser.h"

struct ast_node *prefix(struct lexer *lexer)
{
    if (parser_peek(lexer) == TOKEN_WORD_ASSIGNMENT)
    {
        struct ast_node *current = ast_node_new(AST_WORD_ASSIGNMENT);
        current->value = lexer_peek(lexer).data;
        parser_pop(lexer);
        return current;
    }
    return redirection(lexer);
}


int is_number(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}

struct ast_node *redirection(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_REDIRECTION);
    char *value = lexer_peek(lexer).data;
    if (is_number(value))
    {
        ast_append(current, ast_node_word(value));
    }
    if (parser_peek(lexer) == TOKEN_REDIR)
    {
        current->value = lexer_peek(lexer).data;
        parser_pop(lexer);
        if (parser_peek(lexer) == TOKEN_WORD)
        {
            ast_append(current, ast_node_word(lexer_peek(lexer).data));
            parser_pop(lexer);
            return current;
        }
    }
    ast_free(current);
    return NULL;
}