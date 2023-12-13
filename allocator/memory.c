/*
 User: Redskaber
 Date: 2023/12/8
 Time: 20:05
*/
#include "memory.h"

SystemResource *initSystemResource(int m1, int c1, int g1, int s1, int n1, int f1) {
    SystemResource *newSystemResource = NULL;
    newSystemResource = malloc(sizeof(SystemResource));
    assert(newSystemResource != NULL);
    newSystemResource->memory = createAllocator(m1);
    newSystemResource->cpu = createCpu(c1);
    newSystemResource->gpu = createGpu(g1);
    newSystemResource->swap = createSwap(s1);
    newSystemResource->netWork = n1;
    newSystemResource->file = f1;


    return newSystemResource;
}

void destroySystemResource(SystemResource *systemResource) {
    if (systemResource != NULL) {
        destroyAllocator(systemResource->memory);
        destroyCpu(systemResource->cpu);
        destroyGpu(systemResource->gpu);
        destroySwap(systemResource->swap);
        free(systemResource);
        systemResource = NULL;
    }
}

void displaySystemResource(SystemResource *systemResource) {
    printf_s("#########################################\n");
    printf_s("\tmemory: \n\t\t%d|%d|%d\n",
             systemResource->memory->total, systemResource->memory->used, systemResource->memory->remain);
    printf_s("\tcpu: \n\t\t%d|%d|%d\n",
             systemResource->cpu->total, systemResource->cpu->used, systemResource->cpu->remain);
    printf_s("\tgpu: \n\t\t%d|%d|%d\n",
             systemResource->gpu->total, systemResource->gpu->used, systemResource->gpu->remain);
    printf_s("\tswap: \n\t\t%d|%d|%d\n",
             systemResource->swap->total, systemResource->swap->used, systemResource->swap->remain);
    printf_s("\tnetWork: %d \n", systemResource->netWork);
    printf_s("\tfile: %d \n", systemResource->file);
    printf_s("#########################################\n");
}





