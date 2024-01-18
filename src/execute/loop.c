/**
 * \file loop.c
 * \brief Loop functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "ast/ast.h"
#include "ast_eval.h"

int while_loop(struct ast_node *node, bool logger_enabled)
{
    int status = 0;
    while (match_ast(node->children[0], logger_enabled) == 0)
    {
        if (logger_enabled)
        {
            printf("while loop\n");
        }
        status = match_ast(node->children[1], logger_enabled);
    }
    return status;
}

int until_loop(struct ast_node *node, bool logger_enabled)
{
    int status = 0;
    while (match_ast(node->children[0], logger_enabled) != 0)
    {
        if (logger_enabled)
        {
            printf("until loop\n");
        }
        status = match_ast(node->children[1], logger_enabled);
    }
    return status;
}
int for_loop(struct ast_node *node, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("for loop\n");
    }
    if (node == NULL)
    {
        return -1;
    }
    return 0;
}
