#include "parser.h"

struct ast_node *parse(struct lexer *lexer)
{
    return input(lexer);
}