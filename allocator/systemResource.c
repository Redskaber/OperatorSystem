/*
 User: Redskaber
 Date: 2023/12/8
 Time: 20:05
*/
#include "systemResource.h"


/**
 * @brief Initializes a SystemResource structure.
 *
 * This function allocates memory for a new SystemResource structure and initializes its fields.
 * It creates an Allocator with the provided memory size and uses it to allocate memory for the SystemResource.
 * It then initializes the memory, cpu, gpu, swap, network, and file fields of the SystemResource.
 * Each of these fields is initialized by calling the corresponding create function (createCpu, createGpu, etc.) with the provided size and the Allocator.
 *
 * @param memory The size of the memory to be allocated for the SystemResource.
 * @param cpu The size of the cpu to be created.
 * @param gpu The size of the gpu to be created.
 * @param swap The size of the swap to be created.
 * @param network The size of the network to be created.
 * @param file The size of the file to be created.
 * @return Pointer to the newly created SystemResource structure.
 */
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

/**
 * @brief Destroys a SystemResource structure.
 *
 * This function deallocates the memory used by the SystemResource structure.
 * It first checks if the SystemResource pointer is not NULL.
 * If it is not NULL, it retrieves the Allocator from the SystemResource and uses it to destroy the cpu, gpu, swap, network, and file fields of the SystemResource.
 * It then deallocates the memory used by the SystemResource structure itself and sets the SystemResource pointer to NULL.
 * Finally, it destroys the Allocator.
 *
 * @param systemResource Pointer to the SystemResource structure to be destroyed.
 */
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

/**
 * @brief Displays the details of a SystemResource structure.
 *
 * This function prints the details of the SystemResource structure to the console.
 * It displays the total, used, and remaining amounts for each of the following resources: memory, cpu, gpu, swap, network, and file.
 * Each resource is displayed in the format "total|used|remaining".
 *
 * @param systemResource Pointer to the SystemResource structure to be displayed.
 */
void displaySystemResource(SystemResource *systemResource) {
    printf_s("#########################################\n");
    printf_s("\tmemory: \n\t%10d|%10d|%10d|\n",
             systemResource->memory->total, systemResource->memory->used, systemResource->memory->remain);
    printf_s("\tcpu: \n\t%10d|%10d|%10d|\n",
             systemResource->cpu->total, systemResource->cpu->used, systemResource->cpu->remain);
    printf_s("\tgpu: \n\t%10d|%10d|%10d|\n",
             systemResource->gpu->total, systemResource->gpu->used, systemResource->gpu->remain);
    printf_s("\tswap: \n\t%10d|%10d|%10d|\n",
             systemResource->swap->total, systemResource->swap->used, systemResource->swap->remain);
    printf_s("\tnetWork: \n\t%10d|%10d|%10d|\n",
             systemResource->netWork->total, systemResource->netWork->used, systemResource->netWork->remain);
    printf_s("\tfile: \n\t%10d|%10d|%10d|\n",
             systemResource->file->total, systemResource->file->used, systemResource->file->remain);
    printf_s("#########################################\n");
}

