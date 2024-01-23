#define _POSIX_C_SOURCE 200112L

/**
 * \file builtin.c
 * \brief Builtin functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
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

/**
 * \brief Echo builtin function and returns the exit status.
 * \param node The AST node.
 * \return The exit status of the last command 0 if success, 1 if error.
 */
int echo_fun(struct ast_node *node)
{
    int no_newline = 0;
    int enable_escapes = 0;
    int j = 1;
    for (int i = 1; i < node->children_count; i++)
    {
        if (strlen(handle_word(node->children[i])) >= 1
            && handle_word(node->children[i])[0] == '-')
        {
            if (strspn(handle_word(node->children[i]), "-neE")
                != strlen(handle_word(node->children[i])))
            {
                goto DEFAULT;
            }
            for (size_t k = 1; k < strlen(handle_word(node->children[i])); k++)
            {
                if (handle_word(node->children[i])[k] == 'n')
                    no_newline = 1;
                else if (handle_word(node->children[i])[k] == 'e')
                    enable_escapes = 1;
                else if (handle_word(node->children[i])[k] == 'E')
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
    print_echo(node, enable_escapes, j);
    if (!no_newline)
        putchar('\n');
    fflush(stdout);
    return 0;
}

extern char **environ;

/**
 * \brief Insert an environment variable in alphabetical order.
 * \param env The environment variable to insert.
 * \param insert_env The environment variable array.
 * \return The environment variable array.
*/
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

int export_fun(struct ast_node *node)
{
    if (node->children_count == 1 || !strcmp(node->children[1]->value, "-p"))
    {
        int env_count = 0;
        char **save_environ = NULL;
        for (size_t i = 0; environ[i]; i++)
        {
            env_count++;
            save_environ = realloc(save_environ, sizeof(char *) * (env_count + 1));
            save_environ[i] = environ[i];
        }

        char **environ_alpha = malloc(sizeof(char *) * (env_count + 1));
        for (size_t i = 0; environ[i]; i++)
        {
            environ_alpha[i] = NULL;
        }

        for (int i = 0; i < env_count; i++)
        {
            environ = insert_env(save_environ[i], environ_alpha, env_count);
        }
        environ_alpha[env_count] = NULL;
        for (size_t i = 0; environ_alpha[i]; i++)
        {
            char *var = strtok(environ_alpha[i], "=");
            char *value = strtok(NULL, "\0");
            printf("export %s=\"%s\"\n", var, value);
        }
        free(environ_alpha);
        free(save_environ);
        return 0;
    }

    if (node->children_count != 3)
    {
        fprintf(stderr, "export: too many arguments\n");
        return 1;
    }

    char *var = node->children[1]->value;
    char *value = node->children[2]->value;

    if (setenv(var, value, 1) == -1)
    {
        fprintf(stderr, "export: setenv failed\n");
        return 1;
    }

    printf("export %s=%s\n", var, getenv(var));

    return 0;
}