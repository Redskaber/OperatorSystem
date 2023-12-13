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

typedef struct Gpu {
    int total;
    int used;
    int remain;

    _Bool (*getRunningSpace)(struct Gpu *Gpu, int member);

    _Bool (*freeRunningSpace)(struct Gpu *Gpu, int remember);

    void (*display)(const struct Gpu *Gpu);

} Gpu;

extern Gpu *createGpu(int total);

extern void destroyGpu(Gpu *Gpu);

#endif //OPERATORSYSTEM_MEMORY_GPU_H
