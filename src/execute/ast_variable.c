/**
 * \file ast_variable.c
 * \brief Handle variables.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "ast_eval.h"
#include "hash_map/hash_map.h"
#include "parser/parser.h"

struct hash_map *variables = NULL;

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
    hash_map_insert(variables, key, value, &updated);
}

/**
 * \brief Get a variable from the hash map.
 * \param key The key of the variable.
 * \return The value of the variable or NULL if not found.
*/
char *get_variable(char *key)
{
    key++;
    // gerer les variables envirovment
    init_variables();
    if (variables == NULL)
    {
        return NULL;
    }
    size_t index = hash(key) % variables->size;
    if (variables->data[index] == NULL)
        return "";
    struct pair_list *ind = variables->data[index];
    if (strcmp(ind->key, key) == 0)
    {
        return ind->value;
    }
    while (ind->next != NULL)
    {
        if (strcmp(ind->key, key) == 0)
            return ind->value;
        ind = ind->next;
    }
    return "";
}

/**
 * \brief Evaluate a node from the AST.
 * \param node The AST to evaluate.
 * \return The exit status of the last command 0 if success, 1 if error.
*/
int ast_eval_assignment(struct ast_node *node)
{
    char *key = node->children[0]->value;
    char *value = node->children[1]->value;
    set_variable(key, value);
    return 0;
}

/**
 * \brief Small function for handle variable (just check ast_node type and check the hash table)
 * \param node The AST to evaluate.
 * \return The value of the variable or NULL if not found.
*/
char *handle_word(struct ast_node *node)
{
    if (node->type == AST_WORD)
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
