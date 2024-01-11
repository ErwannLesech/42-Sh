#include "../parser/parser.h"
#include "ast_eval.h"
#include "builtin.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

// true = 0
// false = 1
// error else

struct builtin_function
{
    char *name;
    int (*fun)(struct ast_node *);
};

struct builtin_function builtin[] = {
    {.name = "echo", .fun = echo_fun},
    {.name = "true", .fun = true_fun},
    {.name = "false", .fun = false_fun}
};

int exec_cmd(struct ast_node *node)
{
    int pid = fork();
    if (pid == 0)
    {
        char ** args = malloc(sizeof(char *) * (node->children_count + 1));
        for (int i = 0; i < node->children_count; i++)
        {
            args[i] = node->children[i]->value;
        }
        args[node->children_count] = NULL;
        if (execvp(node->children[0]->value, args)  == -1)
        {
            free(args);
            return 127;
        }
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

int ast_eval_simple_command(struct ast_node *node)
{
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
    default:
        return -1;
    }
}
