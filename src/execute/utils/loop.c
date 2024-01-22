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

int isDirectory(const char *path)
{
    if (path[strlen(path) - 1] != '*')
    {
        // Le chemin pointe vers un répertoire
        return 0;
    }
    char *cleanedString = malloc(sizeof(char) * strlen(path));
    strncpy(cleanedString, path, strlen(path) - 1);
    cleanedString[strlen(path) - 1] = '\0';
    struct stat pathStat;
    if (stat(cleanedString, &pathStat) != 0)
    {
        free(cleanedString);
        return 0;
    }
    free(cleanedString);
    return S_ISDIR(
        pathStat.st_mode); // Vérifiez si le chemin pointe vers un répertoire
}

int case_directory(struct ast_node *node, int i)
{
    int status = 0;
    char *var = node->children[i]->value;
    var[strlen(var) - 1] = '\0';
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;

    dir = opendir(var);

    if (dir == NULL)
    {
        perror("Erreur lors de l'ouverture du dossier");
        return 0;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", var, entry->d_name);

        if (stat(filePath, &fileStat) == 0)
        {
            if (S_ISREG(fileStat.st_mode))
            {
                printf("%s\n", entry->d_name);
            }
        }
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
