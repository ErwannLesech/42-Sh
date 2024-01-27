/**
 * \file ast_eval.c
 * \brief Evaluate the AST.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "ast_eval.h"

#include <fnmatch.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser/parser.h"
#include "utils/builtin.h"

// true = 0
// false = 1
// error else

/**
 * \struct builtin_function
 * \brief Structure representing a builtin function.
 */
struct builtin_function
{
    char *name;
    int (*fun)(struct ast_node *);
};

/**
 * \struct builtin_function
 * \brief Structure representing a builtin function.
 */
struct builtin_function builtin[] = { { .name = "echo", .fun = echo_fun },
                                      { .name = "true", .fun = true_fun },
                                      { .name = "false", .fun = false_fun },
                                      { .name = "export", .fun = export_fun },
                                      { .name = "cd", .fun = cd_fun },
                                      { .name = ".*", .fun = dot_fun } };

/**
 * \brief Evaluate the while loop
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int exec_cmd(struct ast_node *node)
{
    int pid = fork();
    if (pid == 0)
    {
        char **args = malloc(sizeof(char *) * (node->children_count + 1));
        for (int i = 0; i < node->children_count; i++)
        {
            args[i] = handle_word(node->children[i]);
        }
        args[node->children_count] = NULL;
        if (execvp(node->children[0]->value, args) == -1)
        {
            free(args);
            exit(127);
        }
        free(args);
        exit(0);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            status = WEXITSTATUS(status);
            if (status == 127)
            {
                fprintf(stderr, "Command not found\n");
            }
            return status;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

// TO IMPLEMENT REDIR
int ast_command(struct ast_node *node)
{
    return match_ast(node->children[0]);
}

/**
 * \brief Evaluate simple command from ast
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int ast_eval_simple_command(struct ast_node *node)
{
    int save_fd = -1;
    int fd_dup = -1;
    int fd_redir = redir_manager(node, &save_fd, &fd_dup);
    if (fd_redir == -2)
    {
        fprintf(stderr, "Wrong file descriptor\n");
        return 2;
    }
    if (node->children[0]->type == AST_WORD_ASSIGNMENT)
    {
        int return_val = ast_eval_assignment(node);
        if (fd_redir != -1)
        {
            dup2(save_fd, fd_dup);
            close(save_fd);
            close(fd_redir);
        }
        return return_val;
    }
    char *command = handle_word(node->children[0]);

    for (size_t i = 0; i < sizeof(builtin) / sizeof(struct builtin_function);
         i++)
    {
        if (fnmatch(builtin[i].name, command, 0) == 0)
        {
            int return_val = builtin[i].fun(node);
            if (fd_redir != -1)
            {
                dup2(save_fd, fd_dup);
                close(save_fd);
                close(fd_redir);
            }
            return return_val;
        }
    }
    int return_val = exec_cmd(node);
    if (fd_redir != -1)
    {
        dup2(save_fd, fd_dup);
        close(save_fd);
        close(fd_redir);
    }
    return return_val;
}

/**
 * \brief Evaluate condition from ast
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int ast_eval_condition(struct ast_node *node)
{
    int cond = match_ast(node->children[0]);
    if (cond == 0)
    {
        return match_ast(node->children[1]);
    }
    else if (node->children_count == 3)
    {
        return match_ast(node->children[2]);
    }
    else
    {
        return 0;
    }
}

/**
 * \brief Evaluate command list from ast
 * \param node The AST to evaluate.
 * \return The exit status of the last command.
 */
int ast_eval_command_list(struct ast_node *node)
{
    int status = 0;
    for (int i = 0; i < node->children_count; i++)
    {
        status = match_ast(node->children[i]);
    }
    return status;
}

int match_ast(struct ast_node *node)
{
    struct exec_grammar exec[] = {
        [AST_SIMPLE_COMMAND] = { .func = ast_eval_simple_command },
        [AST_CONDITION] = { .func = ast_eval_condition },
        [AST_COMMAND_LIST] = { .func = ast_eval_command_list },
        [AST_EMPTY] = { .func = NULL },
        [AST_WHILE] = { .func = while_loop },
        [AST_UNTIL] = { .func = until_loop },
        [AST_FOR] = { .func = for_loop },
        [AST_AND_OR] = { .func = ast_and_or },
        [AST_PIPELINE] = { .func = pipeline_eval },
        [AST_COMMAND] = { .func = ast_command },
    };

    if (node == NULL)
    {
        return -1;
    }

    if (exec[node->type].func == NULL)
    {
        return 0;
    }

    return exec[node->type].func(node);
}
