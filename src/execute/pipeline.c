#include "../ast/ast.h"
#include "ast_eval.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int pipeline_eval(struct ast_node *node, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("pipeline\n");
    }
    int start = 0;
    if (node->children_count > 0 && node->children[0]->type == AST_NEGATE)
    {
        start = 1;
    }
    int *pipes = malloc(sizeof(int) * (node->children_count));
    if (pipe(pipes) == -1)
    {
        perror("pipe");
        return -1;
    }
    int stat = 0;
    for (int i = start; i < node->children_count; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            if (i != start)
            {
                dup2(pipes[i - 1], STDIN_FILENO);
            }
            if (i != node->children_count - 1)
            {
                dup2(pipes[i], STDOUT_FILENO);
            }
            close(pipes[i]);
            int status = match_ast(node->children[i], logger_enabled);
            exit(status);
        }
        else
        {
            close(pipes[i]);
            int status;
            waitpid(pid, &status, 0);
            stat = WEXITSTATUS(status);
        }
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
        if (node->children[i]->type == AST_WORD 
        && strcmp(node->children[i]->value, "&&") == 0 && status == 1)
        {
           i++;
        }
        else if (node->children[i]->type == AST_WORD 
        && strcmp(node->children[i]->value, "||") == 0 && status == 0)
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
