/*
 User: Redskaber
 Date: 2023/12/16
 Time: 15:43
*/
#include "hashMapResource.h"

/**
 * @brief Creates a new HashMapResource structure.
 *
 * This function allocates memory for a new HashMapResource structure and initializes its fields.
 * It sets the size to the provided size and allocates memory for the table of HashNodeResources.
 * It then initializes all elements of the table to NULL.
 *
 * @param size The size of the HashMapResource.
 * @return Pointer to the newly created HashMapResource structure.
 */
HashMapResource *createHashMapResource(int size) {
    HashMapResource *map = (HashMapResource *) malloc(sizeof(HashMapResource));
    map->size = size;
    map->table = (HashNodeResource **) malloc(sizeof(HashNodeResource *) * size);

    for (int i = 0; i < size; ++i) {
        map->table[i] = NULL;
    }
    return map;
}

/**
 * @brief Computes the hash value for a key.
 *
 * This function computes the hash value for a key using a simple hash function.
 * It iterates over the characters of the key and for each character, it shifts the hash value 5 bits to the left and adds the ASCII value of the character.
 * It then returns the hash value modulo the size of the HashMapResource.
 *
 * @param key The key for which the hash value will be computed.
 * @param size The size of the HashMapResource.
 * @return The computed hash value.
 */
static unsigned int hash(char *key, int size) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % size;
}

/**
 * @brief Inserts a key-value pair into a HashMapResource.
 *
 * This function inserts a key-value pair into a HashMapResource.
 * It first computes the index in the HashMapResource's table by calling the hash function with the key and the size of the HashMapResource.
 * It then allocates memory for a new HashNodeResource, initializes its fields with the provided key and value, and sets its next pointer to NULL.
 * Finally, it inserts the new HashNodeResource at the beginning of the linked list at the computed index in the HashMapResource's table.
 *
 * @param map Pointer to the HashMapResource where the key-value pair will be inserted.
 * @param key The key of the key-value pair to be inserted.
 * @param value The value of the key-value pair to be inserted.
 */
void insertResource(HashMapResource *map, char *key, int *value) {
    unsigned int index = hash(key, map->size);

    HashNodeResource *newNode = (HashNodeResource *) malloc(sizeof(HashNodeResource));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = NULL;

    newNode->next = map->table[index];
    map->table[index] = newNode;
}

/**
 * @brief Retrieves the value associated with a key in a HashMapResource.
 *
 * This function retrieves the value associated with a key in a HashMapResource.
 * It first computes the index in the HashMapResource's table by calling the hash function with the key and the size of the HashMapResource.
 * It then iterates over the linked list at the computed index in the HashMapResource's table and for each HashNodeResource, it checks if the key matches the provided key.
 * If a match is found, it returns the value of the HashNodeResource.
 * If no match is found, it returns NULL.
 *
 * @param map Pointer to the HashMapResource from which the value will be retrieved.
 * @param key The key of the key-value pair to be retrieved.
 * @return The value associated with the key if found, NULL otherwise.
 */
int *getResource(HashMapResource *map, char *key) {
    unsigned int index = hash(key, map->size);
    HashNodeResource *current = map->table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

/**
 * @brief Destroys a HashMapResource structure.
 *
 * This function deallocates the memory used by the HashMapResource structure.
 * It first iterates over the table of the HashMapResource and for each linked list, it deallocates the memory used by the HashNodeResources and their keys.
 * It then deallocates the memory used by the table of the HashMapResource and the HashMapResource structure itself.
 *
 * @param map Pointer to the HashMapResource structure to be destroyed.
 */
void destroyHashMapResource(HashMapResource *map) {
    for (int i = 0; i < map->size; ++i) {
        HashNodeResource *current = map->table[i];
        while (current != NULL) {
            HashNodeResource *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(map->table);
    free(map);
}
