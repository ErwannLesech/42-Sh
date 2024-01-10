#ifndef PARSER_ELEMENT_H
#define PARSER_ELEMENT_H

#include "parser.h"

struct ast_node *input(struct lexer *lexer);
struct ast_node *list(struct lexer *lexer);
struct ast_node *and_or(struct lexer *lexer);
struct ast_node *pipeline(struct lexer *lexer);
struct ast_node *command(struct lexer *lexer);
struct ast_node *simple_command(struct lexer *lexer);
struct ast_node *element(struct lexer *lexer);

#endif