/*
 User: Redskaber
 Date: 2023/12/15
 Time: 21:10
*/
#include "hashMap.h"


/**
 * @brief Creates a new HashMap structure.
 *
 * This function allocates memory for a new HashMap structure and initializes its fields.
 * It sets the size to the provided size and allocates memory for the table of HashNodes.
 * It then initializes all elements of the table to NULL.
 *
 * @param size The size of the HashMap.
 * @return Pointer to the newly created HashMap structure.
 */
HashMap *createHashMap(int size) {
    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    map->size = size;
    map->table = (HashNode **) malloc(sizeof(HashNode *) * size);

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
 * It then returns the hash value modulo the size of the HashMap.
 *
 * @param key The key for which the hash value will be computed.
 * @param size The size of the HashMap.
 * @return The computed hash value.
 */
unsigned int hash(char *key, int size) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % size;
}

/**
 * @brief Inserts a key-value pair into a HashMap.
 *
 * This function inserts a key-value pair into a HashMap.
 * It first computes the index in the HashMap's table by calling the hash function with the key and the size of the HashMap.
 * It then allocates memory for a new HashNode, initializes its fields with the provided key and value, and sets its next pointer to NULL.
 * Finally, it inserts the new HashNode at the beginning of the linked list at the computed index in the HashMap's table.
 *
 * @param map Pointer to the HashMap where the key-value pair will be inserted.
 * @param key The key of the key-value pair to be inserted.
 * @param value The value of the key-value pair to be inserted.
 */
void insert(HashMap *map, char *key, int value) {
    unsigned int index = hash(key, map->size);

    HashNode *newNode = (HashNode *) malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = NULL;

    newNode->next = map->table[index];
    map->table[index] = newNode;
}

/**
 * @brief Retrieves the value associated with a key in a HashMap.
 *
 * This function retrieves the value associated with a key in a HashMap.
 * It first computes the index in the HashMap's table by calling the hash function with the key and the size of the HashMap.
 * It then iterates over the linked list at the computed index in the HashMap's table and for each HashNode, it checks if the key matches the provided key.
 * If a match is found, it returns the value of the HashNode.
 * If no match is found, it returns -1.
 *
 * @param map Pointer to the HashMap from which the value will be retrieved.
 * @param key The key of the key-value pair to be retrieved.
 * @return The value associated with the key if found, -1 otherwise.
 */
int get(HashMap *map, char *key) {
    unsigned int index = hash(key, map->size);
    HashNode *current = map->table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return -1;
}

/**
 * @brief Destroys a HashMap structure.
 *
 * This function deallocates the memory used by the HashMap structure.
 * It first iterates over the table of the HashMap and for each linked list, it deallocates the memory used by the HashNodes and their keys.
 * It then deallocates the memory used by the table of the HashMap and the HashMap structure itself.
 *
 * @param map Pointer to the HashMap structure to be destroyed.
 */
void destroyHashMap(HashMap *map) {
    for (int i = 0; i < map->size; ++i) {
        HashNode *current = map->table[i];
        while (current != NULL) {
            HashNode *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(map->table);
    free(map);
}
