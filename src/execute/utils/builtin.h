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

#endif /* BUILTIN_H */
