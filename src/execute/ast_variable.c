#include "ast_eval.h"
#include "hash_map/hash_map.h"
#include "parser/parser.h"
#include <string.h>

struct hash_map *variables = NULL;

void init_variables()
{
    if (variables == NULL)
    {
        variables = hash_map_init(100);
    }
}

void free_variables()
{
    if (variables != NULL)
        hash_map_free(variables);
}

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
    //hash_map_dump(variables);
}

char *get_variable(char *key)
{
    key++;
    // gerer les variables envirovment
    char *value = hash_map_get(variables, key);
    if (value == NULL)
    {
           // printf("key:%s\n", key);
        return "";
    }
    //printf("value:%s$\n", value);
    //printf("key:%s\n", key);
    return value;
}

int ast_eval_assignment(struct ast_node *node, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("eval_assignment\n");
    }
    //printf("eval_assignment\n");
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

// Small function for handle variable (just check ast_node type and check the
// hash table)
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
