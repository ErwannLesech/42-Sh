#ifndef PARSER_CONDITION_H
#define PARSER_CONDITION_H

#include "parser.h"

struct ast_node *shell_command(struct lexer *lexer);
struct ast_node *rule_if(struct lexer *lexer);
struct ast_node *else_clause(struct lexer *lexer);
struct ast_node *compound_list(struct lexer *lexer);

#endif /* ! PARSER_CONDITION_H*/