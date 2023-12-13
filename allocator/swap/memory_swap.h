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

typedef struct Swap {
    int total;
    int used;
    int remain;

    _Bool (*getRunningSpace)(struct Swap *Swap, int member);

    _Bool (*freeRunningSpace)(struct Swap *Swap, int remember);

    void (*display)(const struct Swap *Swap);

} Swap;

extern Swap *createSwap(int total);

extern void destroySwap(Swap *Swap);

#endif //OPERATORSYSTEM_MEMORY_SWAP_H
