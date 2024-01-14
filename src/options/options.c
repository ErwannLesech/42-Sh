/**
 * \file options.c
 * \brief Options functions.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "options.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../ast/ast.h"

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

void pp_node(struct ast_node *ast, int fd, int *number)
{
    if (!ast)
        return;

    char *buff = malloc(sizeof(char) * 1000);

    write(fd, "node", 4);
    sprintf(buff, "%d", *number);
    buff[strlen(buff)] = '\0';
    write(fd, buff, count_digits(*number));
    write(fd, " [label=\"", 9);
    write(fd, ast_type_to_string(ast->type),
          strlen(ast_type_to_string(ast->type)));
    if (ast->value)
    {
        write(fd, " - ", 3);
        write(fd, ast->value, strlen(ast->value));
    }
    write(fd, "\"];\n", 4);

    int i = 0;
    while (i < ast->children_count && ast->children[i])
    {
        write(fd, "node", 4);
        sprintf(buff, "%d", (*number + i + 11));
        write(fd, buff, count_digits(*number + i + 11));
        write(fd, " [label=\"", 9);
        write(fd, ast_type_to_string(ast->children[i]->type),
              strlen(ast_type_to_string(ast->children[i]->type)));
        if (ast->children[i]->value)
        {
            write(fd, " - ", 3);
            write(fd, ast->children[i]->value, strlen(ast->children[i]->value));
        }
        write(fd, "\"];\n", 4);
        i++;
    }
}

void pp_link(struct ast_node *ast, int fd, int *number)
{
    if (!ast->children)
        return;

    pp_node(ast, fd, number);

    char *buff = malloc(sizeof(char) * 1000);
    int i = 0;

    while (i < ast->children_count && ast->children[i])
    {
        write(fd, "node", 4);
        sprintf(buff, "%d", *number);
        write(fd, buff, count_digits(*number));
        write(fd, " -> ", 4);
        write(fd, "node", 4);
        sprintf(buff, "%d", (*number + i + 11));
        write(fd, buff, count_digits(*number + i + 11));
        write(fd, ";\n", 2);
        i++;
    }
    i = 0;

    while (i < ast->children_count)
    {
        int new = (*number + i + 11);
        pp_link(ast->children[i], fd, &new);
        i++;
    }

    free(buff);
}

void pretty_print(struct ast_node *ast, bool pretty_print_enabled, int *number)
{
    if (!ast || !pretty_print_enabled)
        return;

    int fd = open("pretty_print.gv", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    write(fd, "digraph AST {\n", 14);

    write(fd, "graph [rankdir=TB, ranksep=0.8, nodesep=0.4];\n", 46);
    write(fd, "node [shape=box, color=lightblue, style=filled, fontsize=14];\n",
          62);
    write(fd, "edge [color=black, style=solid, arrowhead=vee];\n\n", 48);

    pp_link(ast, fd, number);

    write(fd, "}\n", 3);
    close(fd);
}