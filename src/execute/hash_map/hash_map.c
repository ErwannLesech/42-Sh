/**
 * \file hash_map.c
 * \brief Implement the hash map.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "hash_map.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hash_map *hash_map_init(size_t size)
{
    struct hash_map *new = malloc(sizeof(struct hash_map));
    new->size = size;
    new->data = malloc(sizeof(struct pair_list *) * size);
    for (size_t i = 0; i < size; i++)
    {
        new->data[i] = NULL;
    }
    return new;
}

bool hash_map_insert(struct hash_map *hash_map, const char *key, char *value,
                     bool *updated)
{
    if (hash_map == NULL || hash_map->size == 0)
    {
        return false;
    }
    size_t index = hash(key) % hash_map->size;
    if (hash_map->data[index] == NULL)
    {
        hash_map->data[index] = malloc(sizeof(struct pair_list));
        hash_map->data[index]->key = key;
        hash_map->data[index]->value = value;
        hash_map->data[index]->next = NULL;
    }
    else
    {
        struct pair_list *ind = hash_map->data[index];
        if (strcmp(ind->key, key) == 0)
        {
            ind->value = value;
            *updated = true;
            return true;
        }
        while (ind->next != NULL)
        {
            if (strcmp(ind->key, key) == 0)
            {
                ind->value = value;
                *updated = true;
                return true;
            }
            ind = ind->next;
        }
        struct pair_list *new = malloc(sizeof(struct pair_list));
        new->key = key;
        new->value = value;
        new->next = hash_map->data[index];
        hash_map->data[index] = new;
    }
    *updated = false;
    return true;
}

void hash_map_free(struct hash_map *hash_map)
{
    if (hash_map == NULL)
    {
        return;
    }
    for (size_t i = 0; i < hash_map->size; i++)
    {
        if (hash_map->data[i] != NULL)
        {
            struct pair_list *index = hash_map->data[i];
            while (index != NULL)
            {
                struct pair_list *to_free = index;
                index = index->next;
                free(to_free);
            }
        }
    }
    free(hash_map->data);
    free(hash_map);
}

void hash_map_dump(struct hash_map *hash_map)
{
    for (size_t i = 0; i < hash_map->size; i++)
    {
        if (hash_map->data[i] != NULL)
        {
            struct pair_list *index = hash_map->data[i];
            while (index->next != NULL)
            {
                printf("%s: %s, ", index->key, index->value);
                index = index->next;
            }
            printf("%s: %s", index->key, index->value);
            printf("\n");
        }
    }
}

char *hash_map_get(const struct hash_map *hash_map, const char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
    {
        return NULL;
    }
    int k = hash(key) % hash_map->size;
    struct pair_list *pair = hash_map->data[k];
    while (pair != NULL)
    {
        if (strcmp(pair->key, key) == 0)
        {
            return pair->value;
        }
        pair = pair->next;
    }
    return NULL;
}

bool hash_map_remove(struct hash_map *hash_map, const char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
    {
        return false;
    }
    int k = hash(key) % hash_map->size;
    struct pair_list *pair = hash_map->data[k];
    if (pair == NULL)
        return false;
    if (strcmp(pair->key, key) == 0)
    {
        struct pair_list *to_free = pair;
        hash_map->data[k] = pair->next;
        free(to_free);
        return true;
    }
    while (pair->next != NULL)
    {
        if (strcmp(pair->next->key, key) == 0)
        {
            struct pair_list *to_free = pair->next;
            pair->next = pair->next->next;
            free(to_free);
            return true;
        }
        pair = pair->next;
    }
    return false;
}