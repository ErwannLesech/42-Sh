#include "builtin.h"

int true_fun(struct ast_node *node)
{
    if (node == NULL)
        return 1;
    return 0;
}

int false_fun(struct ast_node *node)
{
    if (node == NULL)
        return 0;
    return 1;
}

int echo_fun(struct ast_node *node)
{
    for (int i = 1; i < node->children_count; i++)
    {
        printf("%s ", node->children[i]->value);
    }
    printf("\n");
    return 0;
}