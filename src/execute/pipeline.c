#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../ast/ast.h"
#include "ast_eval.h"

int pipeline_eval(struct ast_node *node, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("pipeline\n");
    }
    int start = 0;
    int stat = 0;
    if (node->children_count > 0 && node->children[0]->type == AST_NEGATE)
    {
        start = 1;
    }
    int input_fd = STDIN_FILENO;
    for (int i = start; i < node->children_count; i++)
    {
        int pipes[2];
        if (i != node->children_count)
        {
            if (pipe(pipes) == -1)
            {
                return -1;
            }
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            if (i != start)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (i != node->children_count - 1)
            {
                dup2(pipes[1], STDOUT_FILENO);
                close(pipes[0]);
                close(pipes[1]);
            }
            stat = match_ast(node->children[i], logger_enabled);
            exit(stat);
        }
        if (i != start)
            close(input_fd);
        if (i != node->children_count)
        {
            close(pipes[1]);
            input_fd = pipes[0];
        }
        int status;
        waitpid(pid, &status, 0);
        stat = WEXITSTATUS(status);
    }
    if (start == 1)
    {
        stat = !stat;
    }

    return stat;
}

int ast_and_or(struct ast_node *node, bool logger_enabled)
{
    int status = 0;
    for (int i = 0; i < node->children_count; i++)
    {
        if (node->children[i]->type == AST_AND && status == 1)
        {
            printf("and\n");
            i++;
        }
        else if (node->children[i]->type == AST_OR && status == 0)
        {
            i++;
        }
        else
        {
            status = match_ast(node->children[i], logger_enabled);
        }
    }
    return status;
}
