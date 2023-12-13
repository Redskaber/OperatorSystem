/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/
#include "memory_gpu.h"

static _Bool getRunningSpace(Gpu *Gpu, int member);

static _Bool freeRunningSpace(Gpu *Gpu, int remember);

static void display(const Gpu *Gpu);

Gpu *createGpu(int total) {

    Gpu *newGpu = NULL;
    newGpu = (Gpu *) malloc(sizeof(Gpu));
    assert(newGpu != NULL);

    newGpu->total = total;
    newGpu->used = 0;
    newGpu->remain = total;

    newGpu->getRunningSpace = getRunningSpace;
    newGpu->freeRunningSpace = freeRunningSpace;
    newGpu->display = display;
    return newGpu;
}


static _Bool getRunningSpace(Gpu *Gpu, int member) {
    if (Gpu->used + member <= Gpu->total) {
        Gpu->used += member;
        Gpu->remain -= member;
        return true;
    }
    return false;
}

static _Bool freeRunningSpace(Gpu *Gpu, int remember) {
    if (Gpu->remain + remember <= Gpu->total) {
        Gpu->used -= remember;
        Gpu->remain += remember;
        return true;
    }
    return false;
}

static void display(const Gpu *Gpu) {
    printf_s("########################################\n");
    printf_s("Gpu:\n");
    printf_s("\ttotal: %d\n", Gpu->total);
    printf_s("\tused: %d\n", Gpu->used);
    printf_s("\tremain: %d\n", Gpu->remain);
    printf_s("########################################\n");
}

void destroyGpu(Gpu *Gpu) {
    if (Gpu != NULL) {
        free(Gpu);
    }
}