/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/
#include "memory_swap.h"

static _Bool getRunningSpace(Swap *Swap, int member);

static _Bool freeRunningSpace(Swap *Swap, int remember);

static void display(const Swap *Swap);

Swap *createSwap(int total) {

    Swap *newSwap = NULL;
    newSwap = (Swap *) malloc(sizeof(Swap));
    assert(newSwap != NULL);

    newSwap->total = total;
    newSwap->used = 0;
    newSwap->remain = total;

    newSwap->getRunningSpace = getRunningSpace;
    newSwap->freeRunningSpace = freeRunningSpace;
    newSwap->display = display;
    return newSwap;
}


static _Bool getRunningSpace(Swap *Swap, int member) {
    if (Swap->used + member <= Swap->total) {
        Swap->used += member;
        Swap->remain -= member;
        return true;
    }
    return false;
}

static _Bool freeRunningSpace(Swap *Swap, int remember) {
    if (Swap->remain + remember <= Swap->total) {
        Swap->used -= remember;
        Swap->remain += remember;
        return true;
    }
    return false;
}

static void display(const Swap *Swap) {
    printf_s("########################################\n");
    printf_s("Swap:\n");
    printf_s("\ttotal: %d\n", Swap->total);
    printf_s("\tused: %d\n", Swap->used);
    printf_s("\tremain: %d\n", Swap->remain);
    printf_s("########################################\n");
}

void destroySwap(Swap *Swap) {
    if (Swap != NULL) {
        free(Swap);
    }
}