/**
 * \file builtin.c
 * \brief Builtin functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#define _POSIX_C_SOURCE 200112L
#define PATH_MAX 4096

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

char **remove_at(char **curpath, size_t index)
{
    size_t curpath_len = 0;
    while (curpath[curpath_len] != NULL)
    {
        curpath_len++;
    }
    if (index >= curpath_len)
    {
        return curpath;
    }
    free(curpath[index]);
    curpath[index] = NULL;

    for (size_t i = index; i < curpath_len - 1; i++)
    {
        curpath[i] = curpath[i + 1];
    }
    curpath = realloc(curpath, sizeof(char*) * (curpath_len));
    curpath[curpath_len - 1] = NULL;

    return curpath;
}

int cd_fun(struct ast_node *node)
{
    if (node->children_count > 2)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    char *HOME = getenv("HOME");
    char *OLDPWD = getenv("OLDPWD");
    if (OLDPWD == NULL)
    {
        OLDPWD = "/";
    }
    char *PWD = getenv("PWD");
    if (PWD == NULL)
    {
        PWD = "/";
    }
    char *directory = NULL;
    char *curr_path = NULL;
    if (node->children_count == 1)
    {
        // Step1: The default behavior is implementation-defined and no further
        // steps shall be taken.
        if (!HOME)
        {
            directory = "~";
        }
        // Step2
        else
        {
            directory = HOME;
        }
        curr_path = directory;
    }
    else
    {
        directory = handle_word(node->children[1]);
        
        size_t directory_len = strlen(directory);
        // Step3
        if (directory_len == 0)
        {
            curr_path = HOME;
        }
        else if (directory_len > 0 && directory[0] == '/')
        {
            // Go to step7
            curr_path = directory;
        }
        // Step4
        else if ((directory_len > 0 && directory[0] == '.')
                 || (directory_len > 1 && strcmp(directory, "..") == 0))
        {
            // Go to step6
            // Step6
            curr_path = directory;
        }
        else
            curr_path = directory;
    }
    // Step 7
    size_t curr_path_len = strlen(curr_path);

    char *path = NULL;
    char *curr_pwd = curr_path_len > 0 && curr_path[0] == '-' ? OLDPWD : PWD;
    path = strcpy(malloc(strlen(curr_pwd) + 1), curr_pwd);
    if (curr_path_len > 0 && curr_path[0] == '-')
    {
        printf("%s\n", path);
        fflush(stdout);
    }
    else if (curr_path_len > 0 && curr_path[0] != '/')
    {
        if (PWD[strlen(PWD) - 1] != '/')
        {
            path = realloc(path, strlen(PWD) + 2);
            path[strlen(PWD)] = '/';
            path[strlen(PWD) + 1] = '\0';
        }
        path = realloc(path, strlen(path) + strlen(curr_path) + 1);
        path = strcat(path, curr_path);
    }
    else
    {
        path = realloc(path, strlen(curr_path) + 1);
        path = strcpy(path, curr_path);
    }
    
    // Step8
    char **curpath = malloc(sizeof(char*));
    size_t path_len = strlen(path);
    size_t i = 0;
    size_t curpath_len = 0;
    size_t h = 0;
    //char *preceding_component = NULL;
    while (i < path_len)
    {
        while (i < path_len && path[i] == '/')
        {
            i++;
        }
        h = 0;
        curpath[curpath_len] = malloc(sizeof(char));
        curpath[curpath_len][0] = '\0';
        while (i < path_len && path[i] != '/')
        {
            curpath[curpath_len] = realloc(curpath[curpath_len], sizeof(char) * (h + 2));
            curpath[curpath_len][h] = path[i];
            curpath[curpath_len][h + 1] = '\0';
            h++;
            i++;
        }
        curpath = realloc(curpath, sizeof(char*) * (curpath_len + 2));
        curpath_len++;
    }
    curpath = realloc(curpath, sizeof(char*) * (curpath_len + 1));
    curpath[curpath_len] = NULL;
    for (size_t k = 0; k < curpath_len; k++)
    {
        if (strcmp(curpath[k], ".") == 0)
        {
            curpath = remove_at(curpath, k);
            curpath_len--;
            k--;
        }
    }
    free(path);
    // Debug
    /*
    for (size_t k = 0; k < curpath_len; k++)
    {
        printf("%s\n", curpath[k]);
    }*/

    while (curpath_len > 0)
    {
        if (strcmp(curpath[curpath_len - 1], "..") == 0)
        {
            size_t to_remove = 0;
            while (curpath_len > 0 && strcmp(curpath[curpath_len - 1], "..") == 0)
            {
                curpath = remove_at(curpath, curpath_len - 1);
                curpath_len--;
                to_remove++;
            }
            while (curpath_len > 0 && to_remove > 0)
            {
                curpath = remove_at(curpath, curpath_len - 1);
                curpath_len--;
                to_remove--;
            }
        }
        else
            curpath_len--;
    }

    while (curpath[curpath_len])
    {
        curpath_len++;
    }
    /*printf("curpath_len: %zu\n", curpath_len);
    //Debug
    for (size_t k = 0; k < curpath_len; k++)
    {
        printf("%s\n", curpath[k]); 
    }*/

    if (curpath_len == 0)
    {
        curpath = realloc(curpath, sizeof(char*));
        curpath[0] = malloc(sizeof(char));
        curpath[0][0] = '\0';
        curpath_len++;
    }

    path = malloc(sizeof(char));
    path[0] = '\0';
    size_t j = 0;

    for (size_t k = 0; k < curpath_len; k++)
    {
        path = realloc(path, j + strlen(curpath[k]) + 2);
        path[j] = '/';
        j++;
        for (size_t l = 0; l < strlen(curpath[k]); l++)
        {
            path[j] = curpath[k][l];
            j++;
        }
        if (k == curpath_len - 1)
        {
            path = realloc(path, j + 1);
            path[j] = '\0';
            break;
        }
    }
    if (j > 0 && path[j - 1] == '/' && j > 1)
    {
        path = realloc(path, j);
        path[j - 1] = '\0';
        j--;
    }
    else
    {
        path = realloc(path, j + 1);
        path[j] = '\0';
    }

    for (size_t k = 0; k < curpath_len; k++)
    {
        free(curpath[k]);
    }
    free(curpath);

    if (chdir(path) != 0)
    {
        fprintf(stderr, "cd: %s: No such file or directory\n", path);
        free(path);
        return 1;
    }
    
    setenv("OLDPWD", PWD, 1);
    setenv("PWD", path, 1);

    free(path);
    return 0;
}
