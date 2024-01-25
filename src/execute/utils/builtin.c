/**
 * \file builtin.c
 * \brief Builtin functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#define _POSIX_C_SOURCE 200112L
#define PATH_MAX 4096

#include "ast_eval.h"
#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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


int cd_fun(struct ast_node *node)
{
    if (node->children_count > 2)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    // Get env variables
    char *HOME = getenv("HOME");
    char *PWD = getenv("PWD");
    if (PWD == NULL)
    {
        PWD = "/";
    }

    char *curr_path = handle_word(node->children[0]);
    bool to_free = false;
    // If cd is called without arguments
    if (node->children_count == 1)
    {
        curr_path = HOME;
    }
    else
    {
        curr_path = handle_word(node->children[1]);
        
        size_t len_cr = strlen(curr_path);
        // If cd is called with only '~' or '~/' as argument
        if (len_cr == 0 || (len_cr == 1 && curr_path[0] == '~'))
        {
            curr_path = HOME;
        }
        if (len_cr > 0 && curr_path[0] == '~')
        {
            char *tmp = malloc(sizeof(char) * (strlen(HOME) + len_cr));
            strcpy(tmp, HOME);
            strcat(tmp, curr_path + 1);
            curr_path = tmp;
            to_free = true;
        }
    }
    int return_val = 0;
    char *path = refactor_path(curr_path, true, &return_val);
    if (to_free)
    {
        free(curr_path);
    }
    if (path == NULL)
    {
        return return_val;
    }
    
    // Change the current directory
    if (chdir(path) != 0)
    {
        fprintf(stderr, "cd: %s: No such file or directory\n", path);
        free(path);
        return 1;
    }

    // Update the env variables
    setenv("OLDPWD", PWD, 1);
    setenv("PWD", path, 1);

    free(path);
    return 0;
}

int dot_fun(struct ast_node *node)
{
    if (node->children_count > 2)
    {
        fprintf(stderr, "dot: too many arguments\n");
        return 1;
    }
    char *filename = NULL;
    char *path = NULL;
    // ./file
    if (node->children_count == 1)
    {
        filename = handle_word(node->children[0]);
        size_t filename_len = strlen(filename);
        if (filename_len > 1 && filename[0] == '.' && filename[1] == '/')
        {
            path = malloc(sizeof(char) * (filename_len - 1));
            for (size_t i = 0; i < filename_len - 1; i++)
            {
                path[i] = filename[i + 2];
            }
            path[filename_len - 2] = '\0';
        }
        else
        {
            fprintf(stderr, ". : utilisation : . nom_fichier [arguments]\n");
            return 2;
        }
    }
    // . file
    else
    {
        filename = handle_word(node->children[0]);
        if (strlen(filename) == 1 && filename[0] == '.')
        {
            
            remove_node(node, 0);
            --node->children_count;
            node->children[0] = node->children[1];
            filename = handle_word(node->children[0]);
            path = malloc(sizeof(char) * (strlen(filename) + 1));
            strcpy(path, filename);
        }
        else
        {
            return 2;
        }
    }
    
    char *PWD = getenv("PWD");
    if (PWD == NULL)
    {
        PWD = "/";
    }

    filename = path;
    path = malloc(strlen(PWD) + strlen(filename) + 2);
    for (size_t i = 0; i < strlen(PWD); i++)
    {
        path[i] = PWD[i];
    }
    int j = 0;
    if (PWD[strlen(PWD) - 1] != '/')
    {
        j++;
        path[strlen(PWD)] = '/';
    }

    for (size_t i = 0; i < strlen(filename); i++)
    {
        path[strlen(PWD) + i + j] = filename[i];
    }
    path[strlen(PWD) + strlen(filename) + j] = '\0';
    free(node->children[0]->value);
    free(filename);

    node->children[0]->value = path;
    node->children[1] = NULL;
    int return_val = exec_cmd(node);

    return return_val;
}
