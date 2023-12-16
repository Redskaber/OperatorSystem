/*
 User: Redskaber
 Date: 2023/12/16
 Time: 15:43
*/
#ifndef OPERATORSYSTEM_HASHMAPRESOURCE_H
#define OPERATORSYSTEM_HASHMAPRESOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashNodeResource {
    char *key;
    int *value;
    struct HashNodeResource *next;
} HashNodeResource;

typedef struct HashMapResource {
    int size;
    HashNodeResource **table;
} HashMapResource;

extern HashMapResource *createHashMapResource(int size);

extern void insertResource(HashMapResource *map, char *key, int *value);

extern int *getResource(HashMapResource *map, char *key);

extern void destroyHashMapResource(HashMapResource *map);

#endif //OPERATORSYSTEM_HASHMAPRESOURCE_H
