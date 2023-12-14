/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/
#include "memory_gpu.h"

static _Bool getRunningSpace(Gpu *gpu, int member);

static _Bool freeRunningSpace(Gpu *gpu, int remember);

static void display(const Gpu *gpu);

Gpu *createGpu(int total, Allocator *allocator) {

    Gpu *newGpu = NULL;
    newGpu = allocator->allocate(allocator, sizeof(Gpu));
    assert(newGpu != NULL);

    newGpu->total = total;
    newGpu->used = 0;
    newGpu->remain = total;

    newGpu->getRunningSpace = getRunningSpace;
    newGpu->freeRunningSpace = freeRunningSpace;
    newGpu->display = display;
    return newGpu;
}


static _Bool getRunningSpace(Gpu *gpu, int member) {
    if (gpu->used + member <= gpu->total) {
        gpu->used += member;
        gpu->remain -= member;
        return true;
    }
    return false;
}

static _Bool freeRunningSpace(Gpu *gpu, int remember) {
    if (gpu->remain + remember <= gpu->total) {
        gpu->used -= remember;
        gpu->remain += remember;
        return true;
    }
    return false;
}

static void display(const Gpu *gpu) {
    printf_s("########################################\n");
    printf_s("Gpu:\n");
    printf_s("\ttotal: %d\n", gpu->total);
    printf_s("\tused: %d\n", gpu->used);
    printf_s("\tremain: %d\n", gpu->remain);
    printf_s("########################################\n");
}

void destroyGpu(Gpu *gpu, Allocator *allocator) {
    if (gpu != NULL) {
        allocator->deallocate(allocator, gpu, sizeof(Gpu));
    }
}