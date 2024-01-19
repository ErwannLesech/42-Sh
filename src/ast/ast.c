/**
 * \file ast.c
 * \brief Represent a node of the AST.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "ast.h"

#include "options/options.h"

struct ast_node *ast_node_new(enum ast_type type)
{
    struct ast_node *node = malloc(sizeof(struct ast_node));
    node->type = type;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

struct ast_node *ast_node_word(char *value)
{
    struct ast_node *node = ast_node_new(AST_WORD);
    node->value = value;
    return node;
}

void ast_append(struct ast_node *parent, struct ast_node *child)
{
    if (parent->children == NULL)
    {
        parent->children = malloc(sizeof(struct ast_node *));
    }
    parent->children_count++;
    parent->children = realloc(
        parent->children, sizeof(struct ast_node *) * parent->children_count);
    parent->children[parent->children_count - 1] = child;
}

void ast_free(struct ast_node *node)
{
    if (node == NULL)
        return;
    if (node->children_count != 0)
    {
        for (int i = 0; i < node->children_count; i++)
        {
            ast_free(node->children[i]);
        }
        free(node->children);
    }
    if (node->type == AST_WORD || node->type == AST_WORD_ASSIGNMENT
        || node->type == AST_VARIABLE)
        free(node->value);
    free(node);
}

char *ast_type_to_string(enum ast_type type)
{
    char *strings[] = {
        [AST_SIMPLE_COMMAND] = "AST_SIMPLE_COMMAND",
        [AST_CONDITION] = "AST_CONDITION",
        [AST_COMMAND_LIST] = "AST_COMMAND_LIST",
        [AST_WORD] = "AST_WORD",
        [AST_EMPTY] = "AST_EMPTY",
        [AST_FOR] = "AST_FOR",
        [AST_WHILE] = "AST_WHILE",
        [AST_UNTIL] = "AST_UNTIL",
        [AST_AND_OR] = "AST_AND_OR",
        [AST_NEGATE] = "AST_NEGATE",
        [AST_PIPELINE] = "AST_PIPELINE",
        [AST_REDIRECTION] = "AST_REDIRECTION",
        [AST_AND] = "AST_AND",
        [AST_OR] = "AST_OR",
        [AST_COMMAND] = "AST_COMMAND",
        [AST_WORD_ASSIGNMENT] = "AST_WORD_ASSIGNMENT",
        [AST_VARIABLE] = "AST_VARIABLE",
    };

    if (type < 0 || type > AST_VARIABLE)
        return "AST_UNEXPECTED";

    return strings[type];
}

void print_ast(struct ast_node *node, int depth)
{
    if (node == NULL)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s\n", ast_type_to_string(node->type));
    if (node->type == AST_WORD || node->type == AST_WORD_ASSIGNMENT
        || node->type == AST_VARIABLE)
    {
        printf("%s:\n", node->value);
    }
    for (int i = 0; i < node->children_count; i++)
        print_ast(node->children[i], depth + 1);
}
