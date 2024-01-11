#include "builtin.h"

#include <string.h>

int true_fun(struct ast_node *node)
{
    fflush(stdout);
    if (node == NULL)
        return 1;
    return 0;
}

int false_fun(struct ast_node *node)
{
    fflush(stdout);
    if (node == NULL)
        return 0;
    return 1;
}

int echo_fun(struct ast_node *node)
{
    int no_newline = 0;
    int enable_escapes = 0;
    int j = 1;
    for (int i = 1; i < node->children_count; i++)
    {
        if (node->children[i]->value[0] == '-')
        {
            if (strspn(node->children[i]->value, "-neE") != strlen(node->children[i]->value))
            {
               goto DEFAULT;
            }
            for (size_t k = 1; k < strlen(node->children[i]->value); k++)
            {
                if (node->children[i]->value[k] == 'n')
                    no_newline = 1;
                else if (node->children[i]->value[k] == 'e')
                    enable_escapes = 1;
                else if (node->children[i]->value[k] == 'E')
                    enable_escapes = 0;
                else
                {
                    break;
                }
            }
            j++;
        }
        else
            break;
    }
    DEFAULT:
    for (int i = j; i < node->children_count; i++)
    {
        for (size_t k = 0; k < strlen(node->children[i]->value); k++)
        {
            if (enable_escapes && (node->children[i]->value)[k] == '\\')
            {
                if ((node->children[i]->value)[k + 1] == 'n')
                {
                    putchar('\n');
                    k++;
                }
                else if ((node->children[i]->value)[k + 1] == 't')
                {
                    putchar('\t');
                    k++;
                }
                else if ((node->children[i]->value)[k + 1] == '\\')
                {
                    putchar('\\');
                    k++;
                }
                else
                {
                    putchar('\\');
                    putchar((node->children[i]->value)[k]);
                    k++;
                }
            }
            else
            {
                putchar((node->children[i]->value)[k]);
            }
        }
        if (i != node->children_count - 1)
            putchar(' ');
    }
    if (!no_newline)
        putchar('\n');
    fflush(stdout);
    return 0;
}
