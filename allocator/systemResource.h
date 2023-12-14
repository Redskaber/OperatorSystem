/*
 User: Redskaber
 Date: 2023/12/8
 Time: 20:05
*/
#pragma once
#ifndef OPERATORSYSTEM_SYSTEMRESOURCE_H
#define OPERATORSYSTEM_SYSTEMRESOURCE_H
/*
    // allocate some virtual memory to project.

    - total
    - used
    - remain
 */
#include <assert.h>
#include <stdio.h>
#include "cpu/memory_cpu.h"
#include "file/memory_file.h"
#include "gpu/memory_gpu.h"
#include "memory/memory_allocator.h"
#include "network/memory_network.h"
#include "swap/memory_swap.h"

typedef struct SystemResource {
    Allocator *memory;
    Cpu *cpu;
    Gpu *gpu;
    Swap *swap;
    NetWork *netWork;
    File *file;

} SystemResource;

extern SystemResource *initSystemResource(int memory, int cpu, int gpu, int swap, int network, int file);

extern void destroySystemResource(SystemResource *systemResource);

extern void displaySystemResource(SystemResource *systemResource);

#endif //OPERATORSYSTEM_SYSTEMRESOURCE_H
