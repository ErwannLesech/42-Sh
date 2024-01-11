#include "builtin.h"

#include <string.h>

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
    int no_newline = 0;
    int enable_escapes = 0;
    int j = 1;
    for (int i = 1; i < node->children_count; i++)
    {
        if (strcmp(node->children[i]->value, "-n") == 0)
        {
            j++;
            no_newline = 1;
        }
        else if (strcmp(node->children[i]->value, "-E") == 0)
        {
            j++;
            enable_escapes = 0;
        }
        else if (strcmp(node->children[i]->value, "-e") == 0)
        {
            j++;
            enable_escapes = 1;
        }
        else
        {
            break;
        }
    }
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
    }
    if (!no_newline)
        putchar('\n');
    return 0;
}
