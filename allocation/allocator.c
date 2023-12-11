/*
 User: Redskaber
 Date: 2023/12/7
 Time: 15:13
*/
#include "allocator.h"


SystemResource *initSystemResource(Memory m1, Cpu c1, Gpu g1, Swap s1, NetWork n1, File f1, Allocator *allocator) {
    SystemResource *newSystemResource = NULL;
    newSystemResource = allocator->allocate(allocator, sizeof(SystemResource));
    assert(newSystemResource != NULL);
    newSystemResource->memory = m1;
    newSystemResource->cpu = c1;
    newSystemResource->gpu = g1;
    newSystemResource->swap = s1;
    newSystemResource->netWork = n1;
    newSystemResource->file = f1;

    return newSystemResource;
}

void destroySystemResource(SystemResource *systemResource, Allocator *allocator) {
    if (systemResource != NULL) {
        allocator->deallocate(allocator, systemResource, sizeof(SystemResource));
    }
}

void displaySystemResource(SystemResource *systemResource) {
    printf_s("#########################################\n");
    printf_s("\tmemory: %d \n", systemResource->memory);
    printf_s("\tcpu: %d \n", systemResource->cpu);
    printf_s("\tgpu: %d \n", systemResource->gpu);
    printf_s("\tswap: %d \n", systemResource->swap);
    printf_s("\tnetWork: %d \n", systemResource->netWork);
    printf_s("\tfile: %d \n", systemResource->file);
    printf_s("#########################################\n");
}


BaseAllocate *initBaseAllocate(Allocator *allocator, ResourceType type, int number) {
    BaseAllocate *newBaseAllocate = NULL;

    newBaseAllocate = allocator->allocate(allocator, sizeof(BaseAllocate));
    assert(newBaseAllocate != NULL);

    newBaseAllocate->type = type;
    newBaseAllocate->number = number;

    return newBaseAllocate;
}

void destroyBaseAllocate(BaseAllocate *baseAllocate, Allocator *allocator) {
    if (baseAllocate != NULL) {
        allocator->deallocate(allocator, baseAllocate, sizeof(BaseAllocate));
    }
}

void displayBaseAllocate(BaseAllocate *baseAllocate) {
    printf_s("#########################################\n");
    printf_s("ResourceType: %s\n", resourceTypeToString(baseAllocate->type));
    printf_s("number: %d\n", baseAllocate->number);
    printf_s("#########################################\n");
}

BaseAllocateArr *initBaseAllocateArr(Allocator *allocator, int member) {
    BaseAllocateArr *newBaseAllocateArr = NULL;
    newBaseAllocateArr = allocator->allocate(allocator, sizeof(BaseAllocateArr));
    assert(newBaseAllocateArr != NULL);
    memset(newBaseAllocateArr, 0, sizeof(BaseAllocateArr));

    newBaseAllocateArr->size = member;
    newBaseAllocateArr->array = allocator->allocate(allocator, sizeof(BaseAllocate) * member);
    assert(newBaseAllocateArr->array != NULL);

    return newBaseAllocateArr;
}

void destroyBaseAllocateArr(BaseAllocateArr *baseAllocateArr, Allocator *allocator) {
    if (baseAllocateArr != NULL) {
        allocator->deallocate(allocator, baseAllocateArr->array, sizeof(BaseAllocate) * baseAllocateArr->size);
        allocator->deallocate(allocator, baseAllocateArr, sizeof(BaseAllocateArr));
    }
}


AllocatorResource *initAllocatorResource(Allocator *allocator) {
    AllocatorResource *newAllocatorResource = NULL;

    newAllocatorResource = allocator->allocate(allocator, sizeof(AllocatorResource));
    assert(newAllocatorResource != NULL);

    newAllocatorResource->length = DEFAULT_BASE_ALLOCATE;
    newAllocatorResource->maxResource = initBaseAllocateArr(allocator, newAllocatorResource->length);
    newAllocatorResource->assignedResource = initBaseAllocateArr(allocator, newAllocatorResource->length);
    newAllocatorResource->needResource = initBaseAllocateArr(allocator, newAllocatorResource->length);

    assert(newAllocatorResource->maxResource != NULL);
    assert(newAllocatorResource->assignedResource != NULL);
    assert(newAllocatorResource->needResource != NULL);

    return newAllocatorResource;
}

void destroyAllocatorResource(AllocatorResource *allocatorResource, Allocator *allocator) {
    if (allocatorResource != NULL) {
        destroyBaseAllocateArr(allocatorResource->maxResource, allocator);
        destroyBaseAllocateArr(allocatorResource->assignedResource, allocator);
        destroyBaseAllocateArr(allocatorResource->needResource, allocator);

        allocator->deallocate(allocator, allocatorResource, sizeof(AllocatorResource));
    }
}

void displayAllocatorResource(AllocatorResource *allocatorResource) {
    int length = allocatorResource->length;
    printf_s("#########################################\n");
    printf_s("AllocatorResource:\n");

    printf_s("\tmaxResource:\n");
    for (int i = 0; i < length; ++i) {
        displayBaseAllocate(allocatorResource->maxResource->array[i]);
    }

    printf_s("\tassignedResource:\n");
    for (int i = 0; i < length; ++i) {
        displayBaseAllocate(allocatorResource->assignedResource->array[i]);
    }

    printf_s("\tneedResource:\n");
    for (int i = 0; i < length; ++i) {
        displayBaseAllocate(allocatorResource->needResource->array[i]);
    }
    printf_s("#########################################\n");
}


BankProConBlock *
initBankProConBlock(
        int p_id,
        char *p_name,
        double p_total_time,
        ProcessPriority p_priority,
        CallBack callBack,
        Allocator *allocator
) {
    BankProConBlock *newBankProConBlock = NULL;

    newBankProConBlock = allocator->allocate(allocator, sizeof(BankProConBlock));
    assert(newBankProConBlock != NULL);
    newBankProConBlock->base = initProConBlock(p_id, p_name, p_total_time, p_priority, callBack, allocator);
    newBankProConBlock->resource = initAllocatorResource(allocator);

    return newBankProConBlock;
}


void destroyBankProConBlock(BankProConBlock *bankProConBlock, Allocator *allocator) {
    if (bankProConBlock != NULL) {
        destroyProConBlock(bankProConBlock->base, allocator);
        destroyAllocatorResource(bankProConBlock->resource, allocator);
        allocator->deallocate(allocator, bankProConBlock, sizeof(BankProConBlock));
    }
}


void displayBankProConBlock(BankProConBlock *bankProConBlock) {
    displayProConBlock(bankProConBlock->base);
    displayAllocatorResource(bankProConBlock->resource);
}















