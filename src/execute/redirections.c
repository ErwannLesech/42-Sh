#include "../ast/ast.h"
#include "ast_eval.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int redir_output(struct ast_node *node, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("redir_output\n");
    }
    int stat = 0;
    char *file_name = node->children[node->children_count - 1]->value;
    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        return -1;
    }

    int stdout_fd = dup(STDOUT_FILENO);
    if (stdout_fd == -1)
    {
        return -1;
    }

    // boucle pour dup les différents redirs

    // mettre à null les nodes redirs et réaranger l'arbre

    exec_cmd(node->children[0], false);

    close(fd);
    int status;
    waitpid(pid, &status, 0);
    stat = WEXITSTATUS(status);
    return stat;
}

int redir_manager(struct ast_node *ast, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("redir_manager\n");
    }
    if (ast->children_count < 2)
    {
        return 0;
    }
    return 0;
}