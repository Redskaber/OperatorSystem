/*
 User: Redskaber
 Date: 2023/12/8
 Time: 20:05
*/
#include "memory.h"

static void *allocate(Allocator *allocator, int size);

static void deallocate(Allocator *allocator, void *allocate, int size);

static void display(Allocator *allocator);


Allocator *createAllocator(int total) {
    Allocator *newAllocator = NULL;
    newAllocator = (Allocator *) malloc(sizeof(Allocator));

    newAllocator->total = total;
    newAllocator->used = 0;
    newAllocator->remain = total;

    newAllocator->allocate = allocate;
    newAllocator->deallocate = deallocate;
    newAllocator->display = display;

    return newAllocator;
}


static void *allocate(Allocator *allocator, int size) {

    assert(size <= allocator->remain);

    void *newAllocate = malloc(size);
    memset(newAllocate, 0, size);

    allocator->used += size;
    allocator->remain -= size;

    return newAllocate;
}

static void deallocate(Allocator *allocator, void *allocate, int size) {
    if (allocate != NULL) {
        free(allocate);
        allocator->used -= size;
        allocator->remain += size;
        allocate = NULL;
    }
}

static void display(Allocator *allocator) {
    printf_s("########################################\n");
    printf_s("Allocator:\n");
    printf_s("\ttotal: %d\n", allocator->total);
    printf_s("\tused: %d\n", allocator->used);
    printf_s("\tremain: %d\n", allocator->remain);
    printf_s("########################################\n");
}


void destroy(Allocator *allocator) {
    if (allocator != NULL) {
        free(allocator);
    }
}
