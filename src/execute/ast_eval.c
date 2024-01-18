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

#include "builtin.h"
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
    if (node->children[0]->type == AST_WORD_ASSIGNMENT)
    {
        return ast_eval_assignment(node);
    }
    char *command = node->children[0]->value;
    for (size_t i = 0; i < 3; i++)
    {
        if (strcmp(command, builtin[i].name) == 0)
        {
            return builtin[i].fun(node);
        }
    }
    return exec_cmd(node);
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
    if (node == NULL)
    {
        return -1;
    }
    switch (node->type)
    {
    case AST_SIMPLE_COMMAND:
        return ast_eval_simple_command(node);
    case AST_CONDITION:
        return ast_eval_condition(node);
    case AST_COMMAND_LIST:
        return ast_eval_command_list(node);
    case AST_EMPTY:
        return 0;
    case AST_WHILE:
        return while_loop(node);
    case AST_UNTIL:
        return until_loop(node);
    case AST_FOR:
        return for_loop(node);
    case AST_AND_OR:
        return ast_and_or(node);
    case AST_PIPELINE:
        return pipeline_eval(node);
    case AST_COMMAND:
        return ast_command(node);
    default:
        return -1;
    }
}
