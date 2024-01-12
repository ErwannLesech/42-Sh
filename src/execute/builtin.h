/**
 * \file builtin.h
 * \brief Builtin functions.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
*/

#ifndef BUILTIN_H
#define BUILTIN_H

#include "ast_eval.h"

/**
 * \brief Echo builtin function and returns the exit status.
 */
int echo_fun(struct ast_node *node);

/**
 * \brief True builtin function and returns the exit status.
 */
int true_fun(struct ast_node *node);

/**
 * \brief False builtin function and returns the exit status.
 */
int false_fun(struct ast_node *node);


#endif /* BUILTIN_H */
