/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:09
*/
#include "memory_allocator.h"

static void *allocate(Allocator *allocator, size_t size);

static void *reallocate(Allocator *allocator, void *oldMemory, size_t OldSize, size_t NewSize);

static void deallocate(Allocator *allocator, void *allocate, size_t size);

static void display(const Allocator *allocator);


/**
 * @brief Creates a new Allocator structure.
 *
 * This function allocates memory for a new Allocator structure and initializes its fields.
 * It sets the total, used, and remaining fields to the provided total value, 0, and total value respectively.
 * It also assigns the allocate, reallocate, deallocate, and display functions to the corresponding function pointers in the Allocator structure.
 *
 * @param total The total amount of memory resources.
 * @return Pointer to the newly created Allocator structure.
 */
Allocator *createAllocator(int total) {
    Allocator *newAllocator = NULL;
    newAllocator = (Allocator *) malloc(sizeof(Allocator));
    assert(newAllocator != NULL);

    newAllocator->total = total;
    newAllocator->used = 0;
    newAllocator->remain = total;

    newAllocator->allocate = allocate;
    newAllocator->reallocate = reallocate;
    newAllocator->deallocate = deallocate;
    newAllocator->display = display;

    return newAllocator;
}

/**
 * @brief Allocates memory resources.
 *
 * This function tries to allocate a specified amount of memory resources.
 * It first checks if the requested amount of resources can be allocated without exceeding the total memory resources.
 * If the allocation is possible, it increases the used resources by the requested amount, decreases the remaining resources by the same amount, and returns a pointer to the allocated memory.
 * If the allocation is not possible, it returns NULL.
 *
 * @param allocator Pointer to the Allocator structure representing the memory resources.
 * @param size The amount of memory resources requested.
 * @return Pointer to the allocated memory.
 */
static void *allocate(Allocator *allocator, size_t size) {

    assert(size <= allocator->remain);

    void *newAllocate = malloc(size);
    memset(newAllocate, 0, size);

    allocator->used += (int) size;
    allocator->remain -= (int) size;

    return newAllocate;
}

/**
 * @brief Reallocates memory resources.
 *
 * This function tries to reallocate a specified amount of memory resources.
 * It first reallocates the memory and checks if the reallocation was successful.
 * If the reallocation is successful, it updates the used and remaining resources and returns a pointer to the reallocated memory.
 * If the reallocation is not successful, it returns NULL.
 *
 * @param allocator Pointer to the Allocator structure representing the memory resources.
 * @param oldMemory Pointer to the memory to be reallocated.
 * @param OldSize The current size of the memory.
 * @param NewSize The new size of the memory.
 * @return Pointer to the reallocated memory.
 */
static void *reallocate(Allocator *allocator, void *oldMemory, size_t OldSize, size_t NewSize) {
    void *newMemory = realloc(oldMemory, NewSize);
    assert(newMemory != NULL);
    allocator->used += (int) (NewSize - OldSize);
    allocator->remain -= (int) (NewSize - OldSize);
    return newMemory;
}

/**
 * @brief Deallocates memory resources.
 *
 * This function tries to deallocate a specified amount of memory resources.
 * It first checks if the memory to be deallocated is not NULL.
 * If it is not NULL, it deallocates the memory, updates the used and remaining resources, and sets the memory pointer to NULL.
 *
 * @param allocator Pointer to the Allocator structure representing the memory resources.
 * @param allocate Pointer to the memory to be deallocated.
 * @param size The size of the memory to be deallocated.
 */
static void deallocate(Allocator *allocator, void *allocate, size_t size) {
    if (allocate != NULL) {
        free(allocate);
        allocator->used -= (int) size;
        allocator->remain += (int) size;
        allocate = NULL;
    }
}

/**
 * @brief Displays the details of an Allocator structure.
 *
 * This function prints the details of the Allocator structure to the console.
 * It displays the total, used, and remaining memory resources.
 *
 * @param allocator Pointer to the Allocator structure to be displayed.
 */
static void display(const Allocator *allocator) {
    printf_s("########################################\n");
    printf_s("Allocator:\n");
    printf_s("\ttotal: %d\n", allocator->total);
    printf_s("\tused: %d\n", allocator->used);
    printf_s("\tremain: %d\n", allocator->remain);
    printf_s("########################################\n");
}

/**
 * @brief Destroys an Allocator structure.
 *
 * This function deallocates the memory used by the Allocator structure.
 * It first checks if the Allocator pointer is not NULL.
 * If it is not NULL, it deallocates the memory used by the Allocator structure and sets the Allocator pointer to NULL.
 *
 * @param allocator Pointer to the Allocator structure to be destroyed.
 */
void destroyAllocator(Allocator *allocator) {
    if (allocator != NULL) {
        free(allocator);
    }
}
