/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/
#include "memory_swap.h"

static _Bool getRunningSpace(Swap *Swap, int member);

static _Bool freeRunningSpace(Swap *Swap, int remember);

static void display(const Swap *Swap);


/**
 * @brief Creates a new Swap structure.
 *
 * This function allocates memory for a new Swap structure and initializes its fields.
 * It sets the total, used, and remaining fields to the provided total value, 0, and total value respectively.
 * It also assigns the getRunningSpace, freeRunningSpace, and display functions to the corresponding function pointers in the Swap structure.
 *
 * @param total The total amount of Swap resources.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created Swap structure.
 */
Swap *createSwap(int total, Allocator *allocator) {

    Swap *newSwap = NULL;
    newSwap = allocator->allocate(allocator, sizeof(Swap));
    assert(newSwap != NULL);
    assert(total >= 0);

    newSwap->total = total;
    newSwap->used = 0;
    newSwap->remain = total;

    newSwap->getRunningSpace = getRunningSpace;
    newSwap->freeRunningSpace = freeRunningSpace;
    newSwap->display = display;
    return newSwap;
}

/**
 * @brief Attempts to allocate Swap resources.
 *
 * This function tries to allocate a specified amount of Swap resources.
 * It first checks if the requested amount of resources can be allocated without exceeding the total Swap resources.
 * If the allocation is possible, it increases the used resources by the requested amount, decreases the remaining resources by the same amount, and returns true.
 * If the allocation is not possible, it returns false.
 *
 * @param swap Pointer to the Swap structure representing the Swap resources.
 * @param member The amount of Swap resources requested.
 * @return Boolean value indicating whether the allocation was successful.
 */
static _Bool getRunningSpace(Swap *swap, int member) {
    if (swap->used + member <= swap->total) {
        swap->used += member;
        swap->remain -= member;
        return true;
    }
    return false;
}

/**
 * @brief Attempts to free up Swap resources.
 *
 * This function tries to free up a specified amount of Swap resources.
 * It first checks if the requested amount of resources can be freed without exceeding the total Swap resources.
 * If the freeing up is possible, it decreases the used resources by the requested amount, increases the remaining resources by the same amount, and returns true.
 * If the freeing up is not possible, it returns false.
 *
 * @param swap Pointer to the Swap structure representing the Swap resources.
 * @param remember The amount of Swap resources to be freed.
 * @return Boolean value indicating whether the freeing up was successful.
 */
static _Bool freeRunningSpace(Swap *swap, int remember) {
    if (swap->remain + remember <= swap->total) {
        swap->used -= remember;
        swap->remain += remember;
        return true;
    }
    return false;
}

/**
 * @brief Displays the details of a Swap structure.
 *
 * This function prints the details of the Swap structure to the console.
 * It displays the total, used, and remaining Swap resources.
 *
 * @param swap Pointer to the Swap structure to be displayed.
 */
static void display(const Swap *swap) {
    printf_s("########################################\n");
    printf_s("Swap:\n");
    printf_s("\ttotal: %d\n", swap->total);
    printf_s("\tused: %d\n", swap->used);
    printf_s("\tremain: %d\n", swap->remain);
    printf_s("########################################\n");
}

/**
 * @brief Destroys a Swap structure.
 *
 * This function deallocates the memory used by the Swap structure.
 * It first checks if the Swap pointer is not NULL.
 * If it is not NULL, it uses the provided Allocator to deallocate the memory used by the Swap structure and sets the Swap pointer to NULL.
 *
 * @param swap Pointer to the Swap structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroySwap(Swap *swap, Allocator *allocator) {
    if (swap != NULL) {
        allocator->deallocate(allocator, swap, sizeof(Swap));
    }
}
