/*
 User: Redskaber
 Date: 2023/12/14
 Time: 14:24
*/

#pragma once
#ifndef OPERATORSYSTEM_MEMORY_NETWORK_H
#define OPERATORSYSTEM_MEMORY_NETWORK_H

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>
#include "../memory/memory_allocator.h"

typedef struct NetWork {
    int total;
    int used;
    int remain;

    _Bool (*connect)(struct NetWork *netWork, int member);

    _Bool (*disconnect)(struct NetWork *netWork, int remember);

    void (*display)(const struct NetWork *netWork);
} NetWork;

extern NetWork *createNetWork(int total, Allocator *allocator);

extern void destroyNetWork(NetWork *netWork, Allocator *allocator);

#endif //OPERATORSYSTEM_MEMORY_NETWORK_H
