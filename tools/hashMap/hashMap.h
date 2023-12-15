/*
 User: Redskaber
 Date: 2023/12/15
 Time: 21:10
*/
#ifndef OPERATORSYSTEM_HASHMAP_H
#define OPERATORSYSTEM_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct HashNode {
    char *key;
    int value;
    struct HashNode *next;
} HashNode;

typedef struct HashMap {
    int size;
    HashNode **table;
} HashMap;


extern HashMap *createHashMap(int size);

extern void insert(HashMap *map, char *key, int value);

extern int get(HashMap *map, char *key);

extern void destroyHashMap(HashMap *map);

#endif //OPERATORSYSTEM_HASHMAP_H
