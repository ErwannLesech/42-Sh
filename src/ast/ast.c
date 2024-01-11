#include "ast.h"

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
    if (node->type == AST_WORD)
        free(node->value);
    if (node->children != NULL)
    {
        for (int i = 0; i < node->children_count; i++)
        {
            ast_free(node->children[i]);
        }
        free(node->children);
    }
    free(node);
}

char *ast_type_to_string(enum ast_type type)
{
    switch (type)
    {
    case AST_SIMPLE_COMMAND:
        return "AST_SIMPLE_COMMAND";
    case AST_CONDITION:
        return "AST_CONDITION";
    case AST_COMMAND_LIST:
        return "AST_COMMAND_LIST";
    case AST_WORD:
        return "AST_WORD";
    case AST_EMPTY:
        return "AST_EMPTY";
    default:
        return "UNKNOWN";
    }
}

void print_ast(struct ast_node *node, int depth)
{
    if (node == NULL)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s\n", ast_type_to_string(node->type));
    for (int i = 0; i < node->children_count; i++)
        print_ast(node->children[i], depth + 1);
}
