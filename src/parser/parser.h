#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"

struct ast_node *parse(struct lexer *lexer);
struct ast_node *shell_command(struct lexer *lexer);
struct ast_node *rule_if(struct lexer *lexer);
struct ast_node *else_clause(struct lexer *lexer);
struct ast_node *compound_list(struct lexer *lexer);
struct ast_node *input(struct lexer *lexer);
struct ast_node *list(struct lexer *lexer);
struct ast_node *and_or(struct lexer *lexer);
struct ast_node *pipeline(struct lexer *lexer);
struct ast_node *command(struct lexer *lexer);
struct ast_node *simple_command(struct lexer *lexer);
struct ast_node *element(struct lexer *lexer);
enum token_type parser_peek(struct lexer *lexer);
enum token_type parser_pop(struct lexer *lexer);

#endif /* ! PARSER_H*/
