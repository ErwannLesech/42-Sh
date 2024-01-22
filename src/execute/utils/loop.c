/**
 * \file loop.c
 * \brief Loop functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <err.h>

#include "ast/ast.h"
#include "ast_eval.h"
#include "options/options.h"

int while_loop(struct ast_node *node)
{
    int status = 0;
    while (match_ast(node->children[0]) == 0)
    {
        status = match_ast(node->children[1]);
    }
    return status;
}

int until_loop(struct ast_node *node)
{
    int status = 0;
    while (match_ast(node->children[0]) != 0)
    {
        status = match_ast(node->children[1]);
    }
    return status;
}

int build_num(char *str, int *start)
{
    int build_num = 0;
    while ((size_t)*start < strlen(str) && str[*start] - '0' >= 0
           && str[*start] - '0' < 10)
    {
        build_num = build_num * 10 + (str[*start] - '0');
        (*start)++;
    }
    return build_num;
}

int isValidPattern(const char *input)
{
    if (input[0] == '{' && input[strlen(input) - 1] == '}')
    {
        char *cleanedString = malloc(sizeof(char) * strlen(input));
        strncpy(cleanedString, input + 1, strlen(input) - 2);
        cleanedString[strlen(input) - 2] = '\0';
        char *startStr = strtok(cleanedString, "..");
        char *endStr = strtok(NULL, "..");

        if (startStr != NULL && endStr != NULL && is_number(startStr)
            && is_number(endStr))
        {
            free(cleanedString);
            return 1;
        }
        free(cleanedString);
    }
    return 0;
}

int case_number(struct ast_node *node, int index)
{
    int status = 0;
    char *var = node->children[0]->value;
    int length = 0;
    int *numbers = malloc(sizeof(int) * length);
    int start = 1;
    int first = build_num(node->children[index]->value, &start);
    start += 2;
    int c = 0;
    int last = build_num(node->children[index]->value, &start);
    // printf("last : %d\n", last);
    for (int j = first; j <= last; j++)
    {
        numbers = realloc(numbers, sizeof(int) * (++length));
        numbers[c] = j;
        c++;
    }
    for (int i = 0; i < length; i++)
    {
        char *str = malloc(sizeof(char) * 10);
        sprintf(str, "%d", numbers[i]);
        set_variable(var, str);
        status = match_ast(node->children[node->children_count - 1]);
        free(str);
    }
    free(numbers);
    return status;
}

int isDirectory(char *path)
{
    if (path[strlen(path) - 1] == '*')
        path[strlen(path) - 1] = '\0';
    struct stat fileStat;
    if (stat(path, &fileStat) == -1)
        return 0;
    return S_ISDIR(fileStat.st_mode);
}

int case_directory(struct ast_node *node, int i)
{
    int status = 0;
    char *var = node->children[i]->value;
    DIR *dir = opendir(var);
    if (dir == NULL)
        errx(1, "'%s': No such file or directory", var);
    struct dirent *entry = readdir(dir);
    for (; entry != NULL; entry = readdir(dir))
    {
        if (strcmp(entry->d_name, ".") == 0
               || strcmp(entry->d_name, "..") == 0)
                continue;
        char str[1024];
            strcpy(str, var);
            if (var[strlen(var) - 1] != '/')
                strcat(str, "/");
            strcat(str, entry->d_name);
            printf("%s\n", str);
        }
    closedir(dir);
    return status;
}

int for_loop(struct ast_node *node)
{
    int status = 0;
    char *var = node->children[0]->value;
    if (node->children_count == 2)
    {
        // HANDLE CASE OF $@
        return 0;
    }
    else
    {
        for (int i = 1; i < node->children_count - 1; i++)
        {
            if (isValidPattern(node->children[i]->value))
            {
                status = case_number(node, i);
            }
            else if (isDirectory(node->children[i]->value))
            {
                status = case_directory(node, i);
            }
            else
            {
                set_variable(var, node->children[i]->value);
                status = match_ast(node->children[node->children_count - 1]);
            }
        }
    }
    return status;
}
