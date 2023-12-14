/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/

#pragma once

#ifndef OPERATORSYSTEM_MEMORY_GPU_H
#define OPERATORSYSTEM_MEMORY_GPU_H

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>
#include "../memory/memory_allocator.h"

typedef struct Gpu {
    int total;
    int used;
    int remain;

    _Bool (*getRunningSpace)(struct Gpu *gpu, int member);

    _Bool (*freeRunningSpace)(struct Gpu *gpu, int remember);

    void (*display)(const struct Gpu *gpu);

} Gpu;

extern Gpu *createGpu(int total, Allocator* allocator);

extern void destroyGpu(Gpu *gpu, Allocator* allocator);

#endif //OPERATORSYSTEM_MEMORY_GPU_H
