/**
 * \file ast_eval.h
 * \brief Evaluate the AST.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef AST_EVAL_H
#define AST_EVAL_H

#include <stdbool.h>

#include "../ast/ast.h"
#include "../options/options.h"

/**
 * \brief Evaluates the given AST and returns the exit status of the last
 * command. \param node The AST to evaluate. \param logger_enabled Whether the
 * logger is enabled or not. \return The exit status of the last command.
 */
int match_ast(struct ast_node *node, bool logger_enabled);

#endif /* AST_EVAL_H */
