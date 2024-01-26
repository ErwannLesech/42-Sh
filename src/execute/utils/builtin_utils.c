#define _POSIX_C_SOURCE 200112L
#define PATH_MAX 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast_eval.h"
#include "builtin.h"

/**
 * \brief Print the current working directory.
 * \param node The AST to evaluate.
 * \return The exit status of the last command 0 if success, 1 if error.
 */
void print_echo(struct ast_node *node, int enable_escapes, int j)
{
    for (int i = j; i < node->children_count; i++)
    {
        for (size_t k = 0; k < strlen(handle_word(node->children[i])); k++)
        {
            if (enable_escapes && (handle_word(node->children[i]))[k] == '\\')
            {
                if ((handle_word(node->children[i]))[k + 1] == 'n')
                {
                    putchar('\n');
                    k++;
                }
                else if ((handle_word(node->children[i]))[k + 1] == 't')
                {
                    putchar('\t');
                    k++;
                }
                else if ((handle_word(node->children[i]))[k + 1] == '\\')
                {
                    putchar('\\');
                    k++;
                }
                else
                {
                    putchar('\\');
                    putchar((handle_word(node->children[i]))[k]);
                    k++;
                }
            }
            else
            {
                putchar((handle_word(node->children[i]))[k]);
            }
        }
        if (i != node->children_count - 1
            && node->children[i]->type != AST_VARIABLE
            && node->children[i]->type != AST_WORD_DOUBLE_QUOTE)
            putchar(' ');
    }
}

char **insert_env(char *env, char **env_alpha, int env_count)
{
    int i = 0;
    while (env_alpha[i] && strcmp(env, env_alpha[i]) > 0)
    {
        i++;
    }
    if (env_alpha[i])
    {
        char *tmp = env_alpha[i];
        env_alpha[i] = env;
        env_alpha = insert_env(tmp, env_alpha, env_count);
    }
    else
    {
        env_alpha[i] = env;
    }
    return env_alpha;
}
