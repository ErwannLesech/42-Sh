/**
 * \file builtin.h
 * \brief Header of the builtin functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef BUILTIN_H
#define BUILTIN_H

#include "ast_eval.h"

/**
 * \brief Echo builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
 */
int echo_fun(struct ast_node *node, int c);

/**
 * \brief True builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
 */
int true_fun(struct ast_node *node, int c);

/**
 * \brief False builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
 */
int false_fun(struct ast_node *node, int c);

/**
 * \brief Export builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
 */
int export_fun(struct ast_node *node);

/**
 * \brief Refactor the path this handle the '.', '..' and '~' (and '-' for cd) in the path.
 * \param path The path to refactor.
 * \param cd_builtin If the path is for the cd builtin.
 * \param exit_status The exit status.
 * \return The refactored path. If the return value is NULL,
 * the path is invalid.
 * \example "../test.sh" -> "/home/user/test.sh"
*/
char *refactor_path(char *path, bool cd_builtin, int *exit_status);

/**
 * \brief Cd builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
*/
int cd_fun(struct ast_node *node);

/**
 * \brief Dot builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
*/
int dot_fun(struct ast_node *node);

#endif /* BUILTIN_H */
