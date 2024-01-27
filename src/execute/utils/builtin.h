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
 * \brief Print the current working directory.
 * \param node The AST to evaluate.
 * \return The exit status of the last command 0 if success, 1 if error.
 */
void print_echo(struct ast_node *node, int enable_escapes, int j);

/**
 * \brief Echo builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status of the last command 0 if success, 1 if error.
 */
int echo_fun(struct ast_node *node);

/**
 * \brief True builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
 */
int true_fun(struct ast_node *node);

/**
 * \brief False builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
 */
int false_fun(struct ast_node *node);

/**
 * \brief Insert an environment variable in alphabetical order.
 * \param env The environment variable to insert.
 * \param insert_env The environment variable array.
 * \return The environment variable array.
 */
char **insert_env(char *env, char **env_alpha, int env_count);

/**
 * \brief Export builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status.
 */
int export_fun(struct ast_node *node);

/**
 * \brief Refactor the path this handle the '.', '..' and '~' (and '-' for cd)
 * in the path. \param path The path to refactor. \param cd_builtin If the path
 * is for the cd builtin. \param exit_status The exit status. \return The
 * refactored path. If the return value is NULL, the path is invalid. \example
 * "../test.sh" -> "/home/user/test.sh"
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
