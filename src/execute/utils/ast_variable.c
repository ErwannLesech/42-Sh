/**
 * \file ast_variable.c
 * \brief Handle variables.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <string.h>

#include "ast_eval.h"
#include "environment_variable.h"
#include "hash_map/hash_map.h"
#include "parser/parser.h"

struct hash_map *variables = NULL;

struct environment_function
{
    char *name;
    char *(*fun)();
};

struct environment_function environment[] = {
    { .name = "@", .fun = at_fun },
    { .name = "*", .fun = star_fun },
    { .name = "?", .fun = quest_fun },
    { .name = "$", .fun = dollar_fun },
    { .name = "number", .fun = number_fun },
    { .name = "#", .fun = sharp_fun },
    { .name = "RANDOM", .fun = random_fun },
    { .name = "UID", .fun = uid_fun },
    { .name = "OLDWPD", .fun = oldpwd_fun },
    { .name = "PWD", .fun = pwd_fun },
    { .name = "" }
};

/**
 * \brief Initialize the variables hash map.
 */
void init_variables()
{
    if (variables == NULL)
    {
        variables = hash_map_init(100);
    }
}

/**
 * \brief Free the variables hash map.
 */
void free_variables()
{
    if (variables != NULL)
        hash_map_free(variables);
}

/**
 * \brief Set a variable in the hash map.
 * \param key The key of the variable.
 * \param value The value of the variable.
 */
void set_variable(char *key, char *value)
{
    init_variables();
    bool updated;
    // printf("key:%s\n", key);
    // printf("value:%s\n", value);
    if (strlen(key) == 0 || strlen(value) == 0)
    {
        return;
    }
    char *key_dup = malloc(sizeof(char) * strlen(key) + 1);
    char *value_dup = malloc(sizeof(char) * strlen(value) + 1);
    strcpy(key_dup, key);
    strcpy(value_dup, value);
    hash_map_insert(variables, key_dup, value_dup, &updated);
    // hash_map_dump(variables);
}

char *get_environment_variable(char *key)
{
    char *value = hash_map_get(variables, key);
    if (value == NULL)
    {
        return "";
    }
    return value;
}

/**
 * \brief Get a variable from the hash map.
 * \param key The key of the variable.
 * \return The value of the variable or NULL if not found.
 */
char *get_variable(char *key)
{
    key++;
    for (int i = 0; environment[i].name[0] != '\0'; i++)
    {
        // printf("key:%s\n", key);
        if (strcmp(key, environment[i].name) == 0)
        {
            // printf("key:%s\n", key);
            return environment[i].fun(key);
        }
    }
    return get_environment_variable(key);
}

/**
 * \brief Evaluate a node from the AST.
 * \param node The AST to evaluate.
 * \return The exit status of the last command 0 if success, 1 if error.
 */
int ast_eval_assignment(struct ast_node *node)
{
    char *key = node->children[0]->value;
    if (node->children_count == 1)
    {
        set_variable(key, "");
        return 0;
    }
    char *value = node->children[1]->value;
    set_variable(key, value);
    return 0;
}

/**
 * \brief Small function for handle variable (just check ast_node type and check
 * the hash table) \param node The AST to evaluate. \return The value of the
 * variable or NULL if not found.
 */
char *handle_word(struct ast_node *node)
{
    if (node->type == AST_WORD || node->type == AST_WORD_DOUBLE_QUOTE)
    {
        return node->value;
    }
    else if (node->type == AST_VARIABLE)
    {
        return get_variable(node->value);
    }
    // IMPOSSIBLE
    return NULL;
}
