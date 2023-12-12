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
    printf_s("###################################\n");
    printf_s("\tResourceType: %s\n", resourceTypeToString(baseAllocate->type));
    printf_s("\tnumber: %d\n", baseAllocate->number);
    printf_s("###################################\n");
}

static BaseAllocateArr *initBaseAllocateArr(Allocator *allocator, int member) {
    BaseAllocateArr *newBaseAllocateArr = NULL;
    newBaseAllocateArr = allocator->allocate(allocator, sizeof(BaseAllocateArr));
    assert(newBaseAllocateArr != NULL);

    newBaseAllocateArr->member = 0;
    newBaseAllocateArr->max_member = member;

    newBaseAllocateArr->array = allocator->allocate(allocator, sizeof(BaseAllocate *) * member);
    assert(newBaseAllocateArr->array != NULL);
    memset(newBaseAllocateArr->array, 0, sizeof(BaseAllocate *) * member);

    return newBaseAllocateArr;
}

void destroyBaseAllocateArr(BaseAllocateArr *baseAllocateArr, Allocator *allocator) {
    if (baseAllocateArr != NULL) {
        if (baseAllocateArr->array != NULL) {
            for (int i = 0; i < baseAllocateArr->member; ++i) {
                destroyBaseAllocate(baseAllocateArr->array[i], allocator);
                baseAllocateArr->array[i] = NULL;
            }
        }
        allocator->deallocate(allocator, baseAllocateArr->array, sizeof(BaseAllocate *) * baseAllocateArr->max_member);
        allocator->deallocate(allocator, baseAllocateArr, sizeof(BaseAllocateArr));
    }
}


static AllocatorResource *initAllocatorResource(Allocator *allocator) {
    AllocatorResource *newAllocatorResource = NULL;

    newAllocatorResource = allocator->allocate(allocator, sizeof(AllocatorResource));
    assert(newAllocatorResource != NULL);

    newAllocatorResource->size = DEFAULT_BASE_ALLOCATE;
    newAllocatorResource->maxResource = initBaseAllocateArr(allocator, newAllocatorResource->size);
    newAllocatorResource->assignedResource = initBaseAllocateArr(allocator, newAllocatorResource->size);
    newAllocatorResource->needResource = initBaseAllocateArr(allocator, newAllocatorResource->size);

    assert(newAllocatorResource->maxResource != NULL);
    assert(newAllocatorResource->assignedResource != NULL);
    assert(newAllocatorResource->needResource != NULL);

    return newAllocatorResource;
}

static _Bool upArrCapacity(BaseAllocateArr *destArr, Allocator *allocator) {

    int newMember = destArr->max_member > 0 && destArr->max_member < 10 ? 10 : 2 * destArr->max_member;
    size_t newSize = newMember * sizeof(BaseAllocate *);
    size_t oldSize = destArr->max_member * sizeof(BaseAllocate *);
    BaseAllocate **newArr = allocator->reallocate(allocator, destArr->array, oldSize, newSize);
    if (newArr == NULL) {
        return false;
    }
    destArr->array = newArr;
    destArr->max_member = newMember;

    return true;
}

void pushToResourceArr(BaseAllocateArr *destArr, ResourceType resource[2], Allocator *allocator) {
    if (destArr->member >= destArr->max_member) {
        assert(upArrCapacity(destArr, allocator) != true);
    }
    destArr->array[destArr->member++] = initBaseAllocate(allocator, resource[0], resource[1]);
}


static void initResourceArr(BaseAllocateArr *destArr, ResourceType resourceArr[][2], int rows, Allocator *allocator) {
    if (destArr->member + rows >= destArr->max_member) {
        assert(upArrCapacity(destArr, allocator) != true);
    }

    BaseAllocate *newTemp = NULL;
    for (int i = 0; i < rows; ++i) {
        newTemp = initBaseAllocate(allocator, resourceArr[i][0], resourceArr[i][1]);
        destArr->array[destArr->member++] = newTemp;
    }
}

void initAllocatorResourceArr(
        BankProConBlock *bankProConBlock,
        ResourceType maxResourceArr[][2],
        ResourceType assignedResourceArr[][2],
        int rows,
        Allocator *allocator
) {
    initResourceArr(bankProConBlock->resource->maxResource, maxResourceArr, rows, allocator);
    initResourceArr(bankProConBlock->resource->assignedResource, assignedResourceArr, rows, allocator);

    ResourceType needResourceArr[rows][2] = {};
    for (int i = 0; i < rows; ++i) {
        needResourceArr[i][0] = maxResourceArr[i][0];
        needResourceArr[i][1] = maxResourceArr[i][1] - assignedResourceArr[i][1];
    }
    initResourceArr(bankProConBlock->resource->needResource, needResourceArr, rows, allocator);
}

void displayAllocatorResource(AllocatorResource *allocatorResource) {
    printf_s("###################################\n");
    printf_s("AllocatorResource:\n");

    printf_s("\tmaxResource:\n\t\t");
    BaseAllocate *temp = NULL;
    for (int i = 0; i < allocatorResource->maxResource->member; ++i) {
        temp = allocatorResource->maxResource->array[i];
        printf_s("%s|%d\t", resourceTypeToString(temp->type), temp->number);
    }
    printf_s("\n");

    printf_s("\tassignedResource:\n\t\t");
    for (int i = 0; i < allocatorResource->assignedResource->member; ++i) {
        temp = allocatorResource->assignedResource->array[i];
        printf_s("%s|%d\t", resourceTypeToString(temp->type), temp->number);
    }
    printf_s("\n");

    printf_s("\tneedResource:\n\t\t");
    for (int i = 0; i < allocatorResource->needResource->member; ++i) {
        temp = allocatorResource->needResource->array[i];
        printf_s("%s|%d\t", resourceTypeToString(temp->type), temp->number);
    }
    printf_s("\n");

    printf_s("###################################\n");
}

void destroyAllocatorResource(AllocatorResource *allocatorResource, Allocator *allocator) {
    if (allocatorResource != NULL) {
        destroyBaseAllocateArr(allocatorResource->maxResource, allocator);
        destroyBaseAllocateArr(allocatorResource->assignedResource, allocator);
        destroyBaseAllocateArr(allocatorResource->needResource, allocator);

        allocator->deallocate(allocator, allocatorResource, sizeof(AllocatorResource));
    }
}

BankProConBlock *initBankProConBlock(
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

BankProConBlock *initBankProConBlockUsed(ProConBlock *proConBlock, Allocator *allocator) {
    BankProConBlock *newBankProConBlock = NULL;

    newBankProConBlock = allocator->allocate(allocator, sizeof(BankProConBlock));
    assert(newBankProConBlock != NULL);
    newBankProConBlock->base = proConBlock;
    newBankProConBlock->resource = initAllocatorResource(allocator);

    return newBankProConBlock;
}

void displayBankProConBlock(BankProConBlock *bankProConBlock) {
    displayProConBlock(bankProConBlock->base);
    displayAllocatorResource(bankProConBlock->resource);
}

void destroyBankProConBlock(BankProConBlock *bankProConBlock, Allocator *allocator) {
    if (bankProConBlock != NULL) {
        destroyProConBlock(bankProConBlock->base, allocator);
        destroyAllocatorResource(bankProConBlock->resource, allocator);
        allocator->deallocate(allocator, bankProConBlock, sizeof(BankProConBlock));
    }
}















