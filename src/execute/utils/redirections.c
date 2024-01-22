#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast/ast.h"
#include "ast_eval.h"
#include "options/options.h"

char *check_present_child(struct ast_node *ast, int ionumbers_count)
{
    if (ionumbers_count < ast->children_count)
    {
        return ast->children[ionumbers_count]->value;
    }
    return NULL;
}

char *address_ionumber(struct ast_node *node, int *fd_ionumber,
                       int *ionumbers_count)
{
    char *file_name = NULL;
    if (*fd_ionumber == -1)
    {
        if (node->value[1] == '&')
        {
            *fd_ionumber = STDERR_FILENO;
        }
        else if (node->value[0] == '>')
        {
            *fd_ionumber = STDOUT_FILENO;
        }
        else
        {
            *fd_ionumber = STDIN_FILENO;
        }
        return check_present_child(node, *ionumbers_count);
    }
    // There is an ionumber
    else
    {
        *ionumbers_count += 1;
        return check_present_child(node, *ionumbers_count);
    }

    return file_name;
}

int redir_handling(struct ast_node *node, int *fd_ionumber, int *fd_dup)
{
    char *file_name = NULL;
    int ionumbers_count = 0;

    if ((file_name = address_ionumber(node, fd_ionumber, &ionumbers_count))
        == NULL)
    {
        return -2;
    }

    int fd;

    if (node->value[0] == '>')
    {
        if (node->value[1] == '>')
        {
            fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else if (node->value[1] == '&')
        {
            if (!is_number(file_name))
            {
                fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            else
            {
                fd = atoi(file_name);
                int flags = fcntl(fd, F_GETFD);
                if (flags == -1)
                {
                    // Le descripteur n'est pas ouvert.
                    return -2;
                }
            }
        }
        else
        {
            fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
    }
    else
    {
        if (node->value[1] == '>')
        {
            fd = open(file_name, O_RDWR | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            fd = open(file_name, O_RDONLY);
        }
    }

    if (fd == -1)
    {
        return -1;
    }

    int save_fd = dup(*fd_ionumber);
    if (save_fd == -1)
    {
        return -2;
    }

    if ((*fd_dup = dup2(fd, *fd_ionumber)) == -1)
    {
        return -2;
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
    if (ast->children_count == 0)
    {
        return;
    }

    ast_free(ast->children[index]);
    ast->children[index] = NULL;
}

int redir_manager(struct ast_node *ast, int *save_fd, int *fd_dup)
{
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
            if ((fd_redir =
                     redir_handling(ast->children[i], &io_number, fd_dup))
                == -2)
            {
                return -2;
            }
            remove_node(ast, i);
        }
    }
    ast->children_count -= redirections_count;

    *save_fd = io_number;

    return fd_redir;
}
