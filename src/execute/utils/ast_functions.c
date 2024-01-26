#include "ast_eval.h"
#include "hash_map/hash_map.h"

#include <string.h>

struct hash_map *functions = NULL;

void init_functions()
{
    if (functions == NULL)
    {
        functions = hash_map_init(100);
    }
}

void free_functions()
{
    if (functions != NULL)
        hash_map_free(functions);
}

void set_function(char *key, struct ast_node *value)
{
    init_functions();
    bool updated;
    hash_map_insert(functions, key, value, &updated);
}

struct ast_node *get_function(char *key)
{
    init_functions();
    if (strlen(key) != 0)
        return NULL;
    //return hash_map_get(functions, key);
    return NULL;
}

int ast_eval_function_def(struct ast_node *node, int c)
{
    set_function(node->children[c]->value, node->children[c + 1]);
    return 0;
}