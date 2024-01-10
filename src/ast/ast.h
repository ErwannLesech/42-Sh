#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>

enum ast_type {
    AST_SIMPLE_COMMAND,
    AST_CONDITION,
    AST_COMMAND_LIST,
    AST_WORD,
};

struct ast_node {
    enum ast_type type;
    struct ast_node **children;
    int children_count;
    char *value;
};

struct ast_node *ast_node_new(enum ast_type type);

void ast_append(struct ast_node *parent, struct ast_node *child);

void ast_free(struct ast_node *node);

struct ast_node *ast_node_word(char *value);

void print_ast(struct ast_node *node, int depth);

#endif /* ! AST_H  */