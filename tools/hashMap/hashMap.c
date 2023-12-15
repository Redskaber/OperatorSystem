/*
 User: Redskaber
 Date: 2023/12/15
 Time: 21:10
*/
#include "hashMap.h"


HashMap *createHashMap(int size) {
    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    map->size = size;
    map->table = (HashNode **) malloc(sizeof(HashNode *) * size);

    for (int i = 0; i < size; ++i) {
        map->table[i] = NULL;
    }
    return map;
}

unsigned int hash(char *key, int size) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % size;
}

void insert(HashMap *map, char *key, int value) {
    unsigned int index = hash(key, map->size);

    HashNode *newNode = (HashNode *) malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = NULL;

    newNode->next = map->table[index];
    map->table[index] = newNode;
}


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
