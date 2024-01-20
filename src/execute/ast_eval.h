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
 * \struct exec_grammar
 * \brief Structure representing links between AST types and functions.
 * \param type The type of the AST node.
 * \param func The function to call.
 */
struct exec_grammar
{
    int (*func)(struct ast_node *);
};

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

int redir_manager(struct ast_node *ast, int *save_fd, int *fd_dup);

/**
 * \brief Set variable
 * \param key The key of the variable.
 * \param value The value of the variable.
 */

void set_variable(char *key, char *value);

/**
 * \brief Use for Free Hash Map Variables at the end of the program
 *
 */
void free_variables();

#endif /* AST_EVAL_H */
