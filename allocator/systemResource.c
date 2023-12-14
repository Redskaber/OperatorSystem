/*
 User: Redskaber
 Date: 2023/12/8
 Time: 20:05
*/
#include "systemResource.h"


SystemResource *initSystemResource(int memory, int cpu, int gpu, int swap, int network, int file) {
    SystemResource *newSystemResource = NULL;

    Allocator *allocator = createAllocator(memory);
    newSystemResource = allocator->allocate(allocator, sizeof(SystemResource));
    assert(newSystemResource != NULL);
    newSystemResource->memory = allocator;
    newSystemResource->cpu = createCpu(cpu, allocator);
    newSystemResource->gpu = createGpu(gpu, allocator);
    newSystemResource->swap = createSwap(swap, allocator);
    newSystemResource->netWork = createNetWork(network, allocator);
    newSystemResource->file = createFile(file, allocator);

    return newSystemResource;
}

void destroySystemResource(SystemResource *systemResource) {
    if (systemResource != NULL) {
        Allocator *allocator = systemResource->memory;
        destroyCpu(systemResource->cpu, allocator);
        destroyGpu(systemResource->gpu, allocator);
        destroySwap(systemResource->swap, allocator);
        destroyNetWork(systemResource->netWork, allocator);
        destroyFile(systemResource->file, allocator);
        allocator->deallocate(systemResource->memory, systemResource, sizeof(SystemResource));
        systemResource = NULL;
        destroyAllocator(allocator);
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
    printf_s("\tnetWork: \n\t\t%d|%d|%d\n",
             systemResource->netWork->total, systemResource->netWork->used, systemResource->netWork->remain);
    printf_s("\tfile: \n\t\t%d|%d|%d\n",
             systemResource->file->total, systemResource->file->used, systemResource->file->remain);
    printf_s("#########################################\n");
}

