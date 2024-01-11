#ifndef AST_EVAL_H
#define AST_EVAL_H

#include "../ast/ast.h"

/**
 * \brief Evaluates the given AST and returns the exit status of the last command.
 */
int match_ast(struct ast_node *node);

#endif /* AST_EVAL_H */
