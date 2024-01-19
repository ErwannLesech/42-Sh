/**
 * \file ast_eval.c
 * \brief Evaluate the AST.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "ast_eval.h"

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.h"
#include "parser/parser.h"

// true = 0
// false = 1
// error else

/**
 * \brief Structure representing a builtin function.
 */
struct builtin_function
{
    char *name;
    int (*fun)(struct ast_node *);
};

struct builtin_function builtin[] = { { .name = "echo", .fun = echo_fun },
                                      { .name = "true", .fun = true_fun },
                                      { .name = "false", .fun = false_fun } };

int exec_cmd(struct ast_node *node, bool logger_enabled)
{
    int pid = fork();
    if (pid == 0)
    {
        char **args = malloc(sizeof(char *) * (node->children_count + 1));
        logger("cmd: ", LOGGER_EXEC, logger_enabled);
        for (int i = 0; i < node->children_count; i++)
        {
            args[i] = handle_word(node->children[i]);
            // printf("%s ", args[i]);
            logger(args[i], LOGGER_EXEC, logger_enabled);
        }
        logger("\n", LOGGER_EXEC, logger_enabled);
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
int ast_command(struct ast_node *node, bool logger_enabled)
{
    return match_ast(node->children[0], logger_enabled);
}

int ast_eval_simple_command(struct ast_node *node, bool logger_enabled)
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
        int return_val = ast_eval_assignment(node, logger_enabled);
        if (fd_redir != -1)
        {
            dup2(save_fd, fd_dup);
            close(save_fd);
            close(fd_redir);
        }
        return return_val;
    }
    char *command = node->children[0]->value;
    for (size_t i = 0; i < 3; i++)
    {
        if (strcmp(command, builtin[i].name) == 0)
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
    int return_val = exec_cmd(node, logger_enabled);
    if (fd_redir != -1)
    {
        dup2(save_fd, fd_dup);
        close(save_fd);
        close(fd_redir);
    }
    return return_val;
}

int ast_eval_condition(struct ast_node *node, bool logger_enabled)
{
    int cond = match_ast(node->children[0], logger_enabled);
    if (cond == 0)
    {
        return match_ast(node->children[1], logger_enabled);
    }
    else if (node->children_count == 3)
    {
        return match_ast(node->children[2], logger_enabled);
    }
    else
    {
        return 0;
    }
}

int ast_eval_command_list(struct ast_node *node, bool logger_enabled)
{
    int status = 0;
    for (int i = 0; i < node->children_count; i++)
    {
        status = match_ast(node->children[i], logger_enabled);
    }
    return status;
}

int match_ast(struct ast_node *node, bool logger_enabled)
{
    if (node == NULL)
    {
        return -1;
    }
    switch (node->type)
    {
    case AST_SIMPLE_COMMAND:
        return ast_eval_simple_command(node, logger_enabled);
    case AST_CONDITION:
        return ast_eval_condition(node, logger_enabled);
    case AST_COMMAND_LIST:
        return ast_eval_command_list(node, logger_enabled);
    case AST_EMPTY:
        return 0;
    case AST_WHILE:
        return while_loop(node, logger_enabled);
    case AST_UNTIL:
        return until_loop(node, logger_enabled);
    case AST_FOR:
        return for_loop(node, logger_enabled);
    case AST_AND_OR:
        return ast_and_or(node, logger_enabled);
    case AST_PIPELINE:
        return pipeline_eval(node, logger_enabled);
    case AST_COMMAND:
        return ast_command(node, logger_enabled);
    default:
        return -1;
    }
}