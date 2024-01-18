/**
 * \file ast_eval.h
 * \brief Evaluate the AST.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef AST_EVAL_H
#define AST_EVAL_H

#include <stdbool.h>

#include "ast/ast.h"
#include "options/options.h"

/**
 * \brief Evaluate the given AST
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int match_ast(struct ast_node *node);

/**
 * \brief Evaluate the while loop
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int while_loop(struct ast_node *node);

/**
 * \brief Evaluate the until loop
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
*/
int until_loop(struct ast_node *node);

/**
 * \brief Evaluate the for loop
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
*/
int for_loop(struct ast_node *node);

/**
 * \brief Pipeline evaluation.
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
*/
int pipeline_eval(struct ast_node *node);

/**
 * \brief Evaluate and_or
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int ast_and_or(struct ast_node *node);

/**
 * \brief Evaluate the ast assignement
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int ast_eval_assignment(struct ast_node *node);

/**
 * \brief Evaluate command from ast
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int ast_command(struct ast_node *node);

/**
 * \brief Handle word assignment
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
char *handle_word(struct ast_node *node);

#endif /* AST_EVAL_H */
