/*
 User: Redskaber
 Date: 2023/12/8
 Time: 20:05
*/
#ifndef OPERATORSYSTEM_MEMORY_H
#define OPERATORSYSTEM_MEMORY_H
/*
    // allocate some virtual memory to project.

    - total
    - used
    - remain
 */
#include <stdio.h>
#include <assert.h>
#include "memory/memory_allocator.h"
#include "cpu/memory_cpu.h"
#include "gpu/memory_gpu.h"
#include "swap/memory_swap.h"


typedef enum ResourceType {
    memory,
    cpu,
    gpu,
    swap,
    network,
    file
} ResourceType;


typedef struct SystemResource {
    Allocator *memory;
    Cpu *cpu;
    Gpu *gpu;
    Swap *swap;

    int netWork;
    int file;

} SystemResource;


#define resourceTypeToString(type) _Generic((type), \
    enum ResourceType:                              \
        (type == memory) ? "memory":                \
        (type == cpu) ? "cpu":                      \
        (type == gpu) ? "gpu":                      \
        (type == swap) ?"swap":                     \
        (type == network) ? "network":              \
        (type == file) ? "file":"Unknown"           \
)

extern SystemResource *initSystemResource(int m1, int c1, int g1, int s1, int n1, int f1);

extern void destroySystemResource(SystemResource *systemResource);

extern void displaySystemResource(SystemResource *systemResource);

#endif //OPERATORSYSTEM_MEMORY_H
