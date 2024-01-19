/**
 * \file ast_eval.c
 * \brief Evaluate the AST.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "ast_eval.h"

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils/builtin.h"
#include "parser/parser.h"

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
                                      { .name = "false", .fun = false_fun } };

/**
 * \brief Evaluate the while loop
 * \param node The AST to evaluate.
 * \param logger_enabled Whether the logger is enabled or not.
 * \return The exit status of the last command.
*/
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

/**
 * \brief Evaluate simple command from ast
 * \param node The AST to evaluate.
 * \param logger_enabled Whether the logger is enabled or not.
 * \return The exit status of the last command.
*/
int ast_eval_simple_command(struct ast_node *node, bool logger_enabled)
{
    if (node->children[0]->type == AST_WORD_ASSIGNMENT)
    {
        return ast_eval_assignment(node, logger_enabled);
    }
    char *command = node->children[0]->value;
    for (size_t i = 0; i < 3; i++)
    {
        if (strcmp(command, builtin[i].name) == 0)
        {
            return builtin[i].fun(node);
        }
    }
    return exec_cmd(node, logger_enabled);
}

/**
 * \brief Evaluate condition from ast
 * \param node The AST to evaluate.
 * \param logger_enabled Whether the logger is enabled or not.
 * \return The exit status of the last command.
*/
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

/**
 * \brief Evaluate command list from ast
 * \param node The AST to evaluate.
 * \param logger_enabled Whether the logger is enabled or not.
 * \return The exit status of the last command.
*/
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
