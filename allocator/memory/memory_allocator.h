/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:09
*/
#pragma once

#ifndef OPERATORSYSTEM_MEMORY_ALLOCATOR_H
#define OPERATORSYSTEM_MEMORY_ALLOCATOR_H

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>

#define ALLOCATE_TOTAL_SIZE 1000

typedef struct Allocator {
    int total;
    int used;
    int remain;

    void *(*allocate)(struct Allocator *allocator, size_t size);

    void *(*reallocate)(struct Allocator *allocator, void *oldMemory, size_t OldSize, size_t NewSize);

    void (*deallocate)(struct Allocator *allocator, void *allocate, size_t size);

    void (*display)(const struct Allocator *allocator);

} Allocator;

extern Allocator *createAllocator(int total);

extern void destroyAllocator(Allocator *allocator);

#endif //OPERATORSYSTEM_MEMORY_ALLOCATOR_H
