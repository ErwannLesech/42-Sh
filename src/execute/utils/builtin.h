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
