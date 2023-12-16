/*
 User: Redskaber
 Date: 2023/12/16
 Time: 15:43
*/
#include "hashMapResource.h"

HashMapResource *createHashMapResource(int size) {
    HashMapResource *map = (HashMapResource *) malloc(sizeof(HashMapResource));
    map->size = size;
    map->table = (HashNodeResource **) malloc(sizeof(HashNodeResource *) * size);

    for (int i = 0; i < size; ++i) {
        map->table[i] = NULL;
    }
    return map;
}

static unsigned int hash(char *key, int size) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % size;
}

void insertResource(HashMapResource *map, char *key, int *value) {
    unsigned int index = hash(key, map->size);

    HashNodeResource *newNode = (HashNodeResource *) malloc(sizeof(HashNodeResource));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = NULL;

    newNode->next = map->table[index];
    map->table[index] = newNode;
}

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
