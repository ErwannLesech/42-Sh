#include "parser.h"

struct ast_node *parse_variable(struct lexer *lexer)
{
    if (parser_peek(lexer) == TOKEN_VARIABLE)
    {
        struct ast_node *current = ast_node_new(AST_VARIABLE);
        struct token tok = lexer_pop(lexer);
        current->value = tok.data;
        return current;
    }
    return NULL;
}
