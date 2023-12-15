/*
 User: Redskaber
 Date: 2023/12/7
 Time: 15:13
*/
#include "banker.h"

static void initAvailableResourceAllocate(
        Banker *banker,
        BankProConBlock *bankProConBlock
) {
    BaseAllocateArr *availableResource = banker->availableResource;
    int avaSize = availableResource->member;

    BaseAllocateArr *needResource = bankProConBlock->resource->needResource;
    int needSize = needResource->member;

    BaseAllocate *avaTemp = NULL;
    BaseAllocate *needTemp = NULL;
    for (int i = 0; i < avaSize; ++i) {
        avaTemp = availableResource->array[i];
        for (int j = 0; j < needSize; ++j) {
            needTemp = needResource->array[j];
            if (needTemp->type == avaTemp->type) {
//                avaTemp;
            }
        }
    }
}

void initAllocatorResourceArr(
        BankProConBlock *bankProConBlock,
        ResourceType maxResourceArr[][2],
        ResourceType assignedResourceArr[][2],
        int rows,
        SystemResource *systemResource
) {
    initResourceArr(
            bankProConBlock->resource->maxResource,
            maxResourceArr, rows,
            systemResource->memory);
    initResourceArr(
            bankProConBlock->resource->assignedResource,
            assignedResourceArr, rows,
            systemResource->memory);

    ResourceType needResourceArr[rows][2] = {};
    for (int i = 0; i < rows; ++i) {
        needResourceArr[i][0] = maxResourceArr[i][0];
        needResourceArr[i][1] = maxResourceArr[i][1] - assignedResourceArr[i][1];
    }
    initResourceArr(
            bankProConBlock->resource->needResource,
            needResourceArr, rows,
            systemResource->memory);

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

static void destroyAllocatorResource(AllocatorResource *allocatorResource, Allocator *allocator) {
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

Banker *initBanker(
        ResourceType availableResourceArr[][2],
        int rows,
        SystemResource *systemResource
) {
    Banker *newBanker = NULL;

    newBanker = systemResource->memory->allocate(systemResource->memory, sizeof(Banker));
    assert(newBanker != NULL);

    newBanker->size = 0;
    newBanker->maxSize = BANKER_INIT_ARRAY_MEMBER;
    size_t initSize = newBanker->maxSize * sizeof(BankProConBlock *);
    newBanker->array = systemResource->memory->allocate(systemResource->memory, initSize);
    assert(newBanker->array != NULL);

    newBanker->availableResource = initBaseAllocateArr(systemResource->memory, newBanker->maxSize);

    initResourceArr(
            newBanker->availableResource,
            availableResourceArr, rows,
            systemResource->memory
    );

    return newBanker;
}

void destroyBanker(Banker *banker, SystemResource *systemResource) {
    if (banker != NULL) {
        if (banker->array != NULL) {
            for (int i = 0; i < banker->size; ++i) {
                destroyBankProConBlock(banker->array[i], systemResource->memory);
                banker->array[i] = NULL;
            }
            size_t arrSize = sizeof(BankProConBlock *) * banker->maxSize;
            systemResource->memory->deallocate(systemResource->memory, banker->array, arrSize);
        }
        destroyBaseAllocateArr(banker->availableResource, systemResource->memory);
        systemResource->memory->deallocate(systemResource->memory, banker, sizeof(Banker));
        banker = NULL;
    }
}

static _Bool upCapacityBankerProConBlockArr(Banker *banker, SystemResource *systemResource) {
    Allocator *allocator = systemResource->memory;

    int maxSize = banker->maxSize < 5 ? 5 : 2 * banker->maxSize;
    size_t oldSize = banker->maxSize * sizeof(BankProConBlock *);
    size_t newSize = maxSize * sizeof(BankProConBlock *);

    BankProConBlock **newArr = allocator->reallocate(allocator, banker->array, oldSize, newSize);
    if (newArr == NULL) {
        return false;
    }
    banker->array = newArr;
    banker->maxSize = maxSize;
    return true;
}

void pushProConBlockToBanker(Banker *banker, BankProConBlock *bankProConBlock, SystemResource *systemResource) {
    if (banker->size >= banker->maxSize) {
        assert(upCapacityBankerProConBlockArr(banker, systemResource) == true);
    }
    banker->array[banker->size++] = bankProConBlock;
}

void pushProConBlockArrToBanker(
        Banker *banker,
        ProConBlock **pcbArr,
        int member, int rows,
        ResourceType bankerProConBlockGroup[member][rows][3],
        SystemResource *systemResource
) {
    BankProConBlock *bankProConBlock = NULL;

    ResourceType maxResourceArr[rows][2] = {};
    ResourceType assignedResourceArr[rows][2] = {};

    for (int i = 0; i < member; ++i) {
        bankProConBlock = initBankProConBlockUsed(pcbArr[i], systemResource->memory);
        for (int j = 0; j < rows; ++j) {
            maxResourceArr[j][0] = bankerProConBlockGroup[i][j][0];
            maxResourceArr[j][1] = bankerProConBlockGroup[i][j][1];
            assignedResourceArr[j][0] = bankerProConBlockGroup[i][j][0];
            assignedResourceArr[j][1] = bankerProConBlockGroup[i][j][2];
        }
        initAllocatorResourceArr(bankProConBlock, maxResourceArr, assignedResourceArr, rows, systemResource);
        pushProConBlockToBanker(banker, bankProConBlock, systemResource);
    }
}
