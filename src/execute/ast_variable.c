#include "ast_eval.h"
#include "hash_map/hash_map.h"
#include "../parser/parser.h"

struct hash_map *variables = NULL;

void init_variables()
{
    if (variables == NULL)
    {
        printf("init_variables\n");
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
    hash_map_insert(variables, key, value, &updated);
}

char *get_variable(char *key)
{
    init_variables();
    if (variables == NULL)
        return NULL;
    size_t index = hash(key) % variables->size;
    if (variables->data[index] == NULL)
        return NULL;
    struct pair_list *ind = variables->data[index];
    if (strcmp(ind->key, key) == 0)
        return ind->value;
    while (ind->next != NULL)
    {
        if (strcmp(ind->key, key) == 0)
            return ind->value;
        ind = ind->next;
    }
    return "\n";
}

int ast_eval_assignment(struct ast_node *node, bool logger_enabled)
{
    if (logger_enabled)
    {
        printf("eval_assignment\n");
    }
    char *key = node->children[0]->value;
    char *value = node->children[1]->value;
    set_variable(key, value);
    return 0;
}

// Small function for handle variable (just check ast_node type and check the hash table)
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
    //IMPOSSIBLE
    return NULL;

}
