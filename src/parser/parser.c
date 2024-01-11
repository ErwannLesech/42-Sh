#include "parser.h"

enum token_type parser_peek(struct lexer *lexer)
{
    struct token tok = lexer_peek(lexer);
    free(tok.data);
    return tok.type;
}

enum token_type parser_pop(struct lexer *lexer)
{
    struct token tok = lexer_pop(lexer);
    free(tok.data);
    return tok.type;
}

struct ast_node *parse(struct lexer *lexer)
{
    return input(lexer);
}
