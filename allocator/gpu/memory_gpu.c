/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/
#include "memory_gpu.h"

static _Bool getRunningSpace(Gpu *gpu, int member);

static _Bool freeRunningSpace(Gpu *gpu, int remember);

static void display(const Gpu *gpu);


/**
 * @brief Creates a new Gpu structure.
 *
 * This function allocates memory for a new Gpu structure and initializes its fields.
 * It sets the total, used, and remaining fields to the provided total value, 0, and total value respectively.
 * It also assigns the getRunningSpace, freeRunningSpace, and display functions to the corresponding function pointers in the Gpu structure.
 *
 * @param total The total amount of Gpu resources.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created Gpu structure.
 */
Gpu *createGpu(int total, Allocator *allocator) {

    Gpu *newGpu = NULL;
    newGpu = allocator->allocate(allocator, sizeof(Gpu));
    assert(newGpu != NULL);
    assert(total >= 0);

    newGpu->total = total;
    newGpu->used = 0;
    newGpu->remain = total;

    newGpu->getRunningSpace = getRunningSpace;
    newGpu->freeRunningSpace = freeRunningSpace;
    newGpu->display = display;
    return newGpu;
}

/**
 * @brief Attempts to allocate Gpu resources.
 *
 * This function tries to allocate a specified amount of Gpu resources.
 * It first checks if the requested amount of resources can be allocated without exceeding the total Gpu resources.
 * If the allocation is possible, it increases the used resources by the requested amount, decreases the remaining resources by the same amount, and returns true.
 * If the allocation is not possible, it returns false without modifying the used or remaining resources.
 *
 * @param gpu Pointer to the Gpu structure representing the Gpu resources.
 * @param member The amount of Gpu resources requested.
 * @return Boolean value indicating whether the allocation was successful.
 */
static _Bool getRunningSpace(Gpu *gpu, int member) {
    if (gpu->used + member <= gpu->total) {
        gpu->used += member;
        gpu->remain -= member;
        return true;
    }
    return false;
}

/**
 * @brief Attempts to free up Gpu resources.
 *
 * This function tries to free up a specified amount of Gpu resources.
 * It first checks if the requested amount of resources can be freed without exceeding the total Gpu resources.
 * If the freeing up is possible, it decreases the used resources by the requested amount, increases the remaining resources by the same amount, and returns true.
 * If the freeing up is not possible, it returns false without modifying the used or remaining resources.
 *
 * @param gpu Pointer to the Gpu structure representing the Gpu resources.
 * @param remember The amount of Gpu resources to be freed.
 * @return Boolean value indicating whether the freeing up was successful.
 */
static _Bool freeRunningSpace(Gpu *gpu, int remember) {
    if (gpu->remain + remember <= gpu->total) {
        gpu->used -= remember;
        gpu->remain += remember;
        return true;
    }
    return false;
}

/**
 * @brief Displays the details of a Gpu structure.
 *
 * This function prints the details of the Gpu structure to the console.
 * It displays the total, used, and remaining Gpu resources.
 * Each resource is displayed in the format "total|used|remaining".
 *
 * @param gpu Pointer to the Gpu structure to be displayed.
 */
static void display(const Gpu *gpu) {
    printf_s("########################################\n");
    printf_s("Gpu:\n");
    printf_s("\ttotal: %d\n", gpu->total);
    printf_s("\tused: %d\n", gpu->used);
    printf_s("\tremain: %d\n", gpu->remain);
    printf_s("########################################\n");
}

/**
 * @brief Destroys a Gpu structure.
 *
 * This function deallocates the memory used by the Gpu structure.
 * It first checks if the Gpu pointer is not NULL.
 * If it is not NULL, it uses the provided Allocator to deallocate the memory used by the Gpu structure and sets the Gpu pointer to NULL.
 *
 * @param gpu Pointer to the Gpu structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyGpu(Gpu *gpu, Allocator *allocator) {
    if (gpu != NULL) {
        allocator->deallocate(allocator, gpu, sizeof(Gpu));
    }
}
