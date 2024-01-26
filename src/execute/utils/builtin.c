#define _POSIX_C_SOURCE 200112L

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

int true_fun(struct ast_node *node, int c)
{
    fflush(stdout);
    if (node == NULL && c < 0)
        return 1;
    return 0;
}

int false_fun(struct ast_node *node, int c)
{
    fflush(stdout);
    if (node == NULL && c < 0)
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
int echo_fun(struct ast_node *node, int c)
{
    int no_newline = 0;
    int enable_escapes = 0;
    int j = c;
    for (int i = c; i < node->children_count; i++)
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

// Export builtin part

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

// Cd builtin part

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
        if (len_cr == 0)
        {
            curr_path = HOME;
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

bool contains_char(char *str, char c)
{
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] == c)
        {
            return true;
        }
    }
    return false;
}


/**
 * \brief Try to find if the first arg is an executable in the PATH.
 * \param first_arg The first arg of the command.
 * \return The path of the executable if found, NULL otherwise.
 * \example "ls" -> "/bin/ls"
 */
char *find_executable(char *first_arg)
{
    char* PATH = getenv("PATH");
    if (PATH == NULL)
    {
        fprintf(stderr, "PATH is not set\n");
        return NULL;
    }
    bool found = false;
    char* PATH_copy = malloc(sizeof(char) * (strlen(PATH) + 1));
    strcpy(PATH_copy, PATH);
    char* token = strtok(PATH_copy, ":");
    while (token != NULL)
    {
        char* path = malloc(sizeof(char) * (strlen(token) + strlen(first_arg) + 2));
        strcpy(path, token);
        strcat(path, "/");
        strcat(path, first_arg);
        if (access(path, F_OK) == 0)
        {
            free(first_arg);
            first_arg = path;
            found = true;
            break;
        }
        free(path);
        token = strtok(NULL, ":");
    }
    free(PATH_copy);
    if (!found)
    {
        fprintf(stderr, "%s: file not found\n", first_arg);
        return NULL;
    }
    return first_arg;
}

int is_binary(FILE *fichier)
{
    int c;

    while ((c = fgetc(fichier)) != EOF)
    {
        if (c < 0x20 && c != '\t' && c != '\n' && c != '\r')
        {
            return 1;
        }
    }
    return 0;
}

int check_file(struct ast_node *node, char *first_arg, char *path)
{
    if (access(path, R_OK) != 0)
    {
        fprintf(stderr, "%s: permission denied\n", path);
        free(first_arg);
        node->children[0]->value = path;
        return 1;
    }
    FILE *file = fopen(path, "r");
    if (is_binary(file))
    {
        fprintf(stderr, "%s: cannot execute binary file\n", path);

        free(first_arg);
        node->children[0]->value = path;
        fclose(file);
        return 1;
    }
    fclose(file);

    free(first_arg);
    node->children[0]->value = path;
    return 0;
}

int dot_fun(struct ast_node *node)
{
    int return_val = 0;

    char *first_arg = handle_word(node->children[0]);
    bool doted = false;
    // . file
    if (node->children_count > 0 && strcmp(first_arg, ".") == 0)
    {
        if (node->children_count == 1)
        {
            fprintf(stderr, ". : utilisation : . nom_fichier [arguments]\n");
            return 2;
        }
        
        // Remove the first arg which is '.'
        ast_free(node->children[0]);
        for (int i = 0; i < node->children_count - 1; i++)
        {
            node->children[i] = node->children[i + 1];
        }
        node->children_count--;
        node->children = realloc(node->children, sizeof(struct ast_node *) * node->children_count);
        first_arg = handle_word(node->children[0]);
    
        if (first_arg == NULL)
        {
            return 1;
        }

        if (!contains_char(first_arg, '/'))
        {
            first_arg = find_executable(first_arg);
            if (first_arg == NULL)
            {
                return 1;
            }
        }
        doted = true;
        
    }
    char *path = refactor_path(first_arg, false, &return_val);
    
    if (path == NULL)
    {
        return return_val;
    }
    if (doted && check_file(node, first_arg, path) == 1)
    {
        return 1;
    }
    if (!doted)
        free(path);
    return_val = exec_cmd(node);

    return return_val;
}

