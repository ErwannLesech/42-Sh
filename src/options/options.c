/**
 * \file options.c
 * \brief Options functions.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "options.h"

#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int is_number(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}

bool check_logger(int *argc, char **argv)
{
    bool logger_enabled = false;
    for (int i = 0; i < *argc; i++)
    {
        if (strcmp(argv[i], "--logger") == 0)
        {
            logger_enabled = true;
            for (int j = i; j < *argc - 1; j++)
            {
                argv[j] = argv[j + 1];
            }
            (*argc)--;
        }
    }
    return logger_enabled;
}

bool check_pretty_print(int *argc, char **argv)
{
    bool pretty_print_enabled = false;
    for (int i = 0; i < *argc; i++)
    {
        if (strcmp(argv[i], "--pretty-print") == 0)
        {
            pretty_print_enabled = true;
            for (int j = i; j < *argc - 1; j++)
            {
                argv[j] = argv[j + 1];
            }
            (*argc)--;
        }
    }
    return pretty_print_enabled;
}

void logger(char *str, enum logger_step step, bool logger_enabled)
{
    if (!str || !logger_enabled)
        return;

    switch (step)
    {
    case LOGGER_INPUT:
        printf("Input: %s\n", str);
        break;

    case LOGGER_PARSER:
        printf("Word_value: %s\n", str);
        break;

    case LOGGER_EXEC:
        printf("%s ", str);
        break;

    default:
        break;
    }
}

/**
 * \brief digit counter
 * \param number the number to count digits
 * \return the number of digits
 */
int count_digits(int number)
{
    if (number == 0)
        return 1;
    int count = 0;
    while (number != 0)
    {
        number /= 10;
        ++count;
    }
    return count;
}

void pp_node(struct ast_node *ast, int fd, int *node_count)
{
    char *buff = malloc(sizeof(char) * 100000);

    if (write(fd, "node", 4) == -1)
        return;
    sprintf(buff, "%d", (*node_count));
    buff[strlen(buff)] = '\0';
    if (write(fd, buff, count_digits(*node_count)) == -1)
        return;
    if (write(fd, " [label=\"", 9) == -1)
        return;
    if (write(fd, ast_type_to_string(ast->type),
              strlen(ast_type_to_string(ast->type)))
        == -1)
        return;
    if (ast->value)
    {
        if (write(fd, " - ", 3) == -1)
            return;
        if (write(fd, ast->value, strlen(ast->value)) == -1)
            return;
    }

    if (write(fd, "\"];\n", 3) == -1)
        return;

    free(buff);
}

void pp_link(struct ast_node *ast, int fd, int *node_count, int parent_id)
{
    char *buff = malloc(sizeof(char) * 100000);

    pp_node(ast, fd, node_count);

    if (parent_id != -1)
    {
        if (write(fd, "node", 4) == -1)
            return;
        sprintf(buff, "%d", parent_id);
        if (write(fd, buff, count_digits(parent_id)) == -1)
            return;
        if (write(fd, " -> ", 4) == -1)
            return;
        if (write(fd, "node", 4) == -1)
            return;
        sprintf(buff, "%d", (*node_count));
        if (write(fd, buff, count_digits(*node_count)) == -1)
            return;
        if (write(fd, ";\n", 2) == -1)
            return;
    }

    parent_id = (*node_count);

    for (int i = 0; i < ast->children_count && ast->children[i]; i++)
    {
        (*node_count)++;
        pp_link(ast->children[i], fd, node_count, parent_id);
    }

    free(buff);
}

void pretty_print(struct ast_node *ast, bool pretty_print_enabled, int *number)
{
    if (!ast || !pretty_print_enabled)
        return;

    int fd = open("pretty_print.gv", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (write(fd, "digraph AST {\n", 14) == -1)
        return;

    if (write(fd, "graph [rankdir=TB, ranksep=0.8, nodesep=0.4];\n", 46) == -1)
        return;
    if (write(fd,
              "node [shape=box, color=lightblue, style=filled, fontsize=14];\n",
              62)
        == -1)
        return;
    if (write(fd, "edge [color=black, style=solid, arrowhead=vee];\n\n", 48)
        == -1)
        return;

    pp_link(ast, fd, number, -1);

    if (write(fd, "}\n", 3) == -1)
        return;
    close(fd);
}
