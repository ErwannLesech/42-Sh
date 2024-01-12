/**
 * \file ast.h
 * \brief Represent a node of the AST.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
*/

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include "../options/options.h"

/**
 * \enum ast_type
 * \brief Enumerate the different types of AST nodes.
 */
enum ast_type
{
    AST_SIMPLE_COMMAND,
    AST_CONDITION,
    AST_COMMAND_LIST,
    AST_WORD,
    AST_EMPTY,
    AST_UNEXPECTED,
};

/**
 * \struct ast_node
 * \brief Structure representing a node of the AST.
 */
struct ast_node
{
    enum ast_type type;
    struct ast_node **children;
    int children_count;
    char *value;
};

/**
 * \brief Create a new AST node.
 * \param type The type of the node.
 * \return The new node.
 */
struct ast_node *ast_node_new(enum ast_type type);

/**
 * \brief Append a child to a node.
 * \param parent The parent node.
 * \param child The child node.
 */
void ast_append(struct ast_node *parent, struct ast_node *child);

/**
 * \brief Free an AST node.
 * \param node The node to free.
 */
void ast_free(struct ast_node *node);

/**
 * \brief Create a new AST node of type AST_WORD.
 * \param value The value of the node.
 * \return The new node.
 */
struct ast_node *ast_node_word(char *value);

/**
 * \brief Create a new AST node of type AST_SIMPLE_COMMAND.
 * \param value The value of the node.
 * \return The new node.
 */
void print_ast(struct ast_node *node, int depth, bool logger_enabled);

#endif /* ! AST_H  */
