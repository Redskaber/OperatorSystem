/*
 User: Redskaber
 Date: 2023/12/14
 Time: 18:51
*/
#include "resource_allocate.h"

static _Bool upArrCapacity(BaseAllocateArr *destArr, Allocator *allocator);

BaseAllocate *initBaseAllocate(Allocator *allocator, ResourceType type, int number) {
    BaseAllocate *newBaseAllocate = NULL;

    newBaseAllocate = allocator->allocate(allocator, sizeof(BaseAllocate));
    assert(newBaseAllocate != NULL);

    newBaseAllocate->type = type;
    newBaseAllocate->number = number;

    return newBaseAllocate;
}

void displayBaseAllocate(BaseAllocate *baseAllocate) {
    printf_s("###################################\n");
    printf_s("\tResourceType: %s\n", resourceTypeToString(baseAllocate->type));
    printf_s("\tnumber: %d\n", baseAllocate->number);
    printf_s("###################################\n");
}

void destroyBaseAllocate(BaseAllocate *baseAllocate, Allocator *allocator) {
    if (baseAllocate != NULL) {
        allocator->deallocate(allocator, baseAllocate, sizeof(BaseAllocate));
    }
}


BaseAllocateArr *initBaseAllocateArr(Allocator *allocator, int member) {
    BaseAllocateArr *newBaseAllocateArr = NULL;
    newBaseAllocateArr = allocator->allocate(allocator, sizeof(BaseAllocateArr));
    assert(newBaseAllocateArr != NULL);

    newBaseAllocateArr->member = 0;
    newBaseAllocateArr->max_member = member;

    int initSize = sizeof(BaseAllocate *) * newBaseAllocateArr->max_member;
    newBaseAllocateArr->array = allocator->allocate(allocator, initSize);
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

        int arraySize = sizeof(BaseAllocate *) * baseAllocateArr->max_member;
        allocator->deallocate(allocator, baseAllocateArr->array, arraySize);
        allocator->deallocate(allocator, baseAllocateArr, sizeof(BaseAllocateArr));
    }
}

void initResourceArr(
        BaseAllocateArr *destArr,
        ResourceType resourceArr[][2],
        int rows,
        Allocator *allocator
) {
    if (destArr->member + rows >= destArr->max_member) {
        assert(upArrCapacity(destArr, allocator) != true);
    }

    BaseAllocate *newTemp = NULL;
    for (int i = 0; i < rows; ++i) {
        newTemp = initBaseAllocate(allocator, resourceArr[i][0], resourceArr[i][1]);
        destArr->array[destArr->member++] = newTemp;
    }
}

void displayBaseAllocateArr(BaseAllocateArr *baseAllocateArr) {
    printf_s("###################################\n");
    for (int i = 0; i < baseAllocateArr->member; ++i) {
        printf_s("[%s, %d] ",
                 resourceTypeToString(baseAllocateArr->array[i]->type),
                 baseAllocateArr->array[i]->number
        );
    }
    printf_s("\n");
    printf_s("###################################\n");

}

void pushToResourceArr(BaseAllocateArr *destArr, ResourceType resource[2], Allocator *allocator) {
    if (destArr->member >= destArr->max_member) {
        assert(upArrCapacity(destArr, allocator) != true);
    }
    destArr->array[destArr->member++] = initBaseAllocate(allocator, resource[0], resource[1]);
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

AllocatorResource *initAllocatorResource(Allocator *allocator) {
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
