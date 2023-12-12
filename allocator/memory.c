/*
 User: Redskaber
 Date: 2023/12/8
 Time: 20:05
*/
#include "memory.h"

static void *allocate(Allocator *allocator, int size);

static void *reallocate(Allocator *allocator, void *oldMemory, size_t OldSize, size_t NewSize);

static void deallocate(Allocator *allocator, void *allocate, int size);

static void display(Allocator *allocator);


Allocator *createAllocator(int total) {
    Allocator *newAllocator = NULL;
    newAllocator = (Allocator *) malloc(sizeof(Allocator));

    newAllocator->total = total;
    newAllocator->used = 0;
    newAllocator->remain = total;

    newAllocator->allocate = allocate;
    newAllocator->reallocate = reallocate;
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

static void *reallocate(Allocator *allocator, void *oldMemory, size_t OldSize, size_t NewSize) {
    void *newMemory = realloc(oldMemory, NewSize);
    assert(newMemory != NULL);
    allocator->used += (int) (NewSize - OldSize);
    allocator->remain -= (int) (NewSize - OldSize);
    return newMemory;
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
