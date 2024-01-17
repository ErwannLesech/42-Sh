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

#include "ast/ast.h"
#include "options/options.h"

/**
 * \brief Evaluates the given AST and returns the exit status of the last
 * command. \param node The AST to evaluate. \param logger_enabled Whether the
 * logger is enabled or not. \return The exit status of the last command.
 */
int match_ast(struct ast_node *node, bool logger_enabled);
int while_loop(struct ast_node *node, bool logger_enabled);
int until_loop(struct ast_node *node, bool logger_enabled);
int for_loop(struct ast_node *node, bool logger_enabled);
int pipeline_eval(struct ast_node *node, bool logger_enabled);
int ast_and_or(struct ast_node *node, bool logger_enabled);
int ast_eval_assignment(struct ast_node *node, bool logger_enabled);
int ast_command(struct ast_node *node, bool logger_enabled);
char *handle_word(struct ast_node *node);
#endif /* AST_EVAL_H */
