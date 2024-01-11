#ifndef AST_EVAL_H
#define AST_EVAL_H

#include "../ast/ast.h"
#include "../options/options.h"

#include <stdbool.h>

/**
 * \brief Evaluates the given AST and returns the exit status of the last command.
 */
int match_ast(struct ast_node *node, bool logger_enabled);

#endif /* AST_EVAL_H */
