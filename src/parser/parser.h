#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"

#include "parser_element.h"
#include "parser_condition.h"

struct ast_node *parse(struct lexer *lexer);

#endif /* ! PARSER_H*/