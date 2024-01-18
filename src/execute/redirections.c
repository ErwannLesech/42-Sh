#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast/ast.h"
#include "ast_eval.h"

int redir_handling(struct ast_node *node, int *fd_ionumber, int *fd_dup, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("redir_output\n");
    }

    char *file_name = NULL;

    if (*fd_ionumber == -1)
    {
        if (node->value[0] == '>')
        {
            *fd_ionumber = STDOUT_FILENO;
        }
        else
        {
            *fd_ionumber = STDIN_FILENO;
        }
        file_name = node->children[0]->value;
    }
    else
    {
        file_name = node->children[1]->value;
    }

    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        return -1;
    }

    int save_fd = dup(*fd_ionumber);
    if (save_fd == -1)
    {
        return -1;
    }

    if ((*fd_dup = dup2(fd, *fd_ionumber)) == -1)
    {
        return -1;
    }

    // mettre à null les nodes redirs et réaranger l'arbre

    // Restore stdout

    *fd_ionumber = save_fd;

    return fd;
}

/**
 * \brief Remove a node from the ast
 * \param ast The ast
 * \param index The index of the node to remove
*/
void remove_node(struct ast_node *ast, int index)
{
    free(ast->children[index]->value);
    free(ast->children[index]);
    ast->children[index] = NULL;
}

int redir_manager(struct ast_node *ast, int *save_fd, int *fd_dup, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("redir_manager\n");
    }
    int fd_redir = -1;
    int io_number = -1;
    int redirections_count = 0;
    for (int i = 0; i < ast->children_count; i++)
    {
        if (ast->children[i]->type == AST_REDIRECTION)
        {
            redirections_count++;
            if (io_number != -1)
            {
                dup2(io_number, *fd_dup);
                close(fd_redir);
                close(io_number);
                io_number = -1;
                fd_redir = -1;
            }
            if (ast->children[i]->children[0]->type == AST_IONUMBER)
            {
                io_number = atoi(ast->children[i]->children[0]->value);
            }
            if ((fd_redir = redir_handling(ast->children[i], &io_number, fd_dup, logger_enabled)) == -1)
            {
                return -1;
            }
            remove_node(ast, i);
        }
    }
    ast->children_count -= redirections_count;

    *save_fd = io_number;

    return fd_redir;
}