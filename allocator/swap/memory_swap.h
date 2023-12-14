/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/

#pragma once

#ifndef OPERATORSYSTEM_MEMORY_SWAP_H
#define OPERATORSYSTEM_MEMORY_SWAP_H

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>
#include "../memory//memory_allocator.h"

typedef struct Swap {
    int total;
    int used;
    int remain;

    _Bool (*getRunningSpace)(struct Swap *swap, int member);

    _Bool (*freeRunningSpace)(struct Swap *swap, int remember);

    void (*display)(const struct Swap *swap);

} Swap;

extern Swap *createSwap(int total, Allocator* allocator);

extern void destroySwap(Swap *swap, Allocator* allocator);

#endif //OPERATORSYSTEM_MEMORY_SWAP_H
