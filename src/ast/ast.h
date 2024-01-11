#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include "../options/options.h"

enum ast_type
{
    AST_SIMPLE_COMMAND,
    AST_CONDITION,
    AST_COMMAND_LIST,
    AST_WORD,
    AST_EMPTY,
    AST_UNEXPECTED,
};

struct ast_node
{
    enum ast_type type;
    struct ast_node **children;
    int children_count;
    char *value;
};

struct ast_node *ast_node_new(enum ast_type type);

void ast_append(struct ast_node *parent, struct ast_node *child);

void ast_free(struct ast_node *node);

struct ast_node *ast_node_word(char *value);

void print_ast(struct ast_node *node, int depth, bool logger_enabled);

#endif /* ! AST_H  */
