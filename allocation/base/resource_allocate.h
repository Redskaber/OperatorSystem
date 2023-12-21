/*
 User: Redskaber
 Date: 2023/12/14
 Time: 18:51
*/
#pragma once
#ifndef OPERATORSYSTEM_RESOURCE_ALLOCATE_H
#define OPERATORSYSTEM_RESOURCE_ALLOCATE_H
#define  DEFAULT_BASE_ALLOCATE 5

#include <stdbool.h>
#include "../../allocator/memory/memory_allocator.h"


typedef enum ResourceType {
    memory,
    cpu,
    gpu,
    swap,
    network,
    file
} ResourceType;

typedef struct BaseAllocate {
    ResourceType type;
    int number;
} BaseAllocate;

typedef struct BaseAllocateArr {
    BaseAllocate **array;
    int max_member;
    int member;
} BaseAllocateArr;

// 动态分配
typedef struct AllocatorResource {
    BaseAllocateArr *maxResource;
    BaseAllocateArr *assignedResource;
    BaseAllocateArr *needResource;
    int size;
} AllocatorResource;

#define resourceTypeToString(type) _Generic((type), \
    enum ResourceType:                              \
        (type == memory) ? "memory":                \
        (type == cpu) ? "cpu":                      \
        (type == gpu) ? "gpu":                      \
        (type == swap) ?"swap":                     \
        (type == network) ? "network":              \
        (type == file) ? "file":"Unknown"           \
)


extern BaseAllocate *initBaseAllocate(Allocator *allocator, ResourceType type, int number);

extern void displayBaseAllocate(BaseAllocate *baseAllocate);

extern void destroyBaseAllocate(BaseAllocate *baseAllocate, Allocator *allocator);

extern BaseAllocateArr *initBaseAllocateArr(Allocator *allocator, int member);

extern BaseAllocateArr *deepCopyBaseAllocateArr(BaseAllocateArr *baseAllocateArr, Allocator *allocator);

extern void pushToBaseAllocateArr(BaseAllocateArr *destArr, BaseAllocate *resource, Allocator *allocator);

extern void pushToBaseAllocateArrByType(BaseAllocateArr *destArr, ResourceType type, int number, Allocator *allocator);

extern void destroyBaseAllocateArr(BaseAllocateArr *baseAllocateArr, Allocator *allocator);

extern void initResourceArr(
        BaseAllocateArr *destArr,
        int rows,
        ResourceType resourceArr[rows][2],
        Allocator *allocator
);

extern void pushToResourceArr(
        BaseAllocateArr *destArr,
        ResourceType resource[2],
        Allocator *allocator
);

extern void pushToResourceArrUsedBaseAllocate(BaseAllocateArr *destArr, BaseAllocate *resource, Allocator *allocator);

extern void displayBaseAllocateArr(BaseAllocateArr *baseAllocateArr);

extern void displayResourceTypArr(int member, ResourceType resourceTypeArr[member][2]);

extern AllocatorResource *initAllocatorResource(Allocator *allocator);

#endif //OPERATORSYSTEM_RESOURCE_ALLOCATE_H
