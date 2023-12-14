/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/
#include "memory_swap.h"

static _Bool getRunningSpace(Swap *Swap, int member);

static _Bool freeRunningSpace(Swap *Swap, int remember);

static void display(const Swap *Swap);

Swap *createSwap(int total, Allocator *allocator) {

    Swap *newSwap = NULL;
    newSwap = allocator->allocate(allocator, sizeof(Swap));
    assert(newSwap != NULL);

    newSwap->total = total;
    newSwap->used = 0;
    newSwap->remain = total;

    newSwap->getRunningSpace = getRunningSpace;
    newSwap->freeRunningSpace = freeRunningSpace;
    newSwap->display = display;
    return newSwap;
}


static _Bool getRunningSpace(Swap *swap, int member) {
    if (swap->used + member <= swap->total) {
        swap->used += member;
        swap->remain -= member;
        return true;
    }
    return false;
}

static _Bool freeRunningSpace(Swap *swap, int remember) {
    if (swap->remain + remember <= swap->total) {
        swap->used -= remember;
        swap->remain += remember;
        return true;
    }
    return false;
}

static void display(const Swap *swap) {
    printf_s("########################################\n");
    printf_s("Swap:\n");
    printf_s("\ttotal: %d\n", swap->total);
    printf_s("\tused: %d\n", swap->used);
    printf_s("\tremain: %d\n", swap->remain);
    printf_s("########################################\n");
}

void destroySwap(Swap *swap, Allocator *allocator) {
    if (swap != NULL) {
        allocator->deallocate(allocator, swap, sizeof(Swap));
    }
}