/*
 User: Redskaber
 Date: 2023/12/8
 Time: 20:05
*/
#ifndef OPERATORSYSTEM_MEMORY_H
#define OPERATORSYSTEM_MEMORY_H
/*
    // allocate some virtual memory to project.

    - total
    - used
    - remain
 */
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>


typedef struct Allocator {
    int total;
    int used;
    int remain;

    void *(*allocate)(struct Allocator *allocator, int size);

    void *(*reallocate)(struct Allocator *allocator, void *oldMemory, size_t OldSize, size_t NewSize);

    void (*deallocate)(struct Allocator *allocator, void *allocate, int size);

    void (*display)(struct Allocator *allocator);

} Allocator;

extern Allocator *createAllocator(int total);

extern void destroy(Allocator *allocator);

#endif //OPERATORSYSTEM_MEMORY_H
