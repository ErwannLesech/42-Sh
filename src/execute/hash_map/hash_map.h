/**
 * \file hash_map.h
 * \brief Header of the hash map.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

/**
 * \struct pair_list
 * \brief Structure representing a pair of key/value.
 */
struct pair_list
{
    const char *key;
    char *value;
    struct pair_list *next;
};

/**
 * \struct hash_map
 * \brief Structure representing a hash map.
 */
struct hash_map
{
    struct pair_list **data;
    size_t size;
};

/**
 * \brief Hash a string.
 * \param str The string to hash.
 * \return The hash.
*/
size_t hash(const char *str);

/**
 * \brief Initialize a hash map.
 * \param size The size of the hash map.
 * \return The new hash map.
 */
struct hash_map *hash_map_init(size_t size);

/**
 * \brief Insert a new pair in the hash map.
 * \param hash_map The hash map.
 * \param key The key of the pair.
 * \param value The value of the pair.
 * \param updated A pointer to a boolean that will be set to true if the value
 * was updated.
 * \return True if the pair was inserted, false otherwise.
 */
bool hash_map_insert(struct hash_map *hash_map, const char *key, char *value,
                     bool *updated);

/**
 * \brief Free a hash map.
 * \param hash_map The hash map to free.
 */
void hash_map_free(struct hash_map *hash_map);

/**
 * \brief Dump a hash map.
 * \param hash_map The hash map to dump.
 */
void hash_map_dump(struct hash_map *hash_map);

/**
 * \brief Get a value from a hash map.
 * \param hash_map The hash map.
 * \param key The key of the pair.
 * \return The value of the pair.
 */
char *hash_map_get(const struct hash_map *hash_map, const char *key);

/**
 * \brief Remove a pair from a hash map.
 * \param hash_map The hash map.
 * \param key The key of the pair.
 * \return True if the pair was removed, false otherwise.
 */
bool hash_map_remove(struct hash_map *hash_map, const char *key);

#endif /* ! HASH_MAP_H */