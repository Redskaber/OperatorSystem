/*
 User: Redskaber
 Date: 2023/12/14
 Time: 18:51
*/
#include "resource_allocate.h"

static _Bool upArrCapacity(BaseAllocateArr *destArr, Allocator *allocator);


/**
 * @brief Initializes a BaseAllocate structure.
 *
 * This function allocates memory for a new BaseAllocate structure and initializes its fields.
 * It assigns the provided resource type and number to the BaseAllocate structure.
 *
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @param type The resource type to be assigned to the BaseAllocate.
 * @param number The resource number to be assigned to the BaseAllocate.
 * @return Pointer to the newly created BaseAllocate structure.
 */
BaseAllocate *initBaseAllocate(Allocator *allocator, ResourceType type, int number) {
    BaseAllocate *newBaseAllocate = NULL;

    newBaseAllocate = allocator->allocate(allocator, sizeof(BaseAllocate));
    assert(newBaseAllocate != NULL);

    newBaseAllocate->type = type;
    newBaseAllocate->number = number;

    return newBaseAllocate;
}

/**
 * @brief Displays a BaseAllocate structure.
 *
 * This function prints the details of the BaseAllocate structure to the console.
 * It displays the resource type and number.
 *
 * @param baseAllocate Pointer to the BaseAllocate structure.
 */
void displayBaseAllocate(BaseAllocate *baseAllocate) {
    printf_s("###################################\n");
    printf_s("\tResourceType: %s\n", resourceTypeToString(baseAllocate->type));
    printf_s("\tnumber: %d\n", baseAllocate->number);
    printf_s("###################################\n");
}

/**
 * @brief Destroys a BaseAllocate structure.
 *
 * This function deallocates the memory used by the BaseAllocate structure.
 * It first checks if the BaseAllocate pointer is not NULL.
 * If it is not NULL, it deallocates the memory used by the BaseAllocate structure.
 *
 * @param baseAllocate Pointer to the BaseAllocate structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyBaseAllocate(BaseAllocate *baseAllocate, Allocator *allocator) {
    if (baseAllocate != NULL) {
        allocator->deallocate(allocator, baseAllocate, sizeof(BaseAllocate));
    }
}

/**
 * @brief Initializes a BaseAllocateArr structure.
 *
 * This function allocates memory for a new BaseAllocateArr structure and initializes its fields.
 * It sets the member to 0 and the max_member to the provided member.
 * It also allocates memory for the array of BaseAllocate pointers and initializes all elements to NULL.
 *
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @param member The initial maximum number of members in the BaseAllocateArr.
 * @return Pointer to the newly created BaseAllocateArr structure.
 */
BaseAllocateArr *initBaseAllocateArr(Allocator *allocator, int member) {
    BaseAllocateArr *newBaseAllocateArr = NULL;
    newBaseAllocateArr = allocator->allocate(allocator, sizeof(BaseAllocateArr));
    assert(newBaseAllocateArr != NULL);

    newBaseAllocateArr->member = 0;
    newBaseAllocateArr->max_member = member;

    size_t initSize = sizeof(BaseAllocate *) * newBaseAllocateArr->max_member;
    newBaseAllocateArr->array = allocator->allocate(allocator, initSize);
    assert(newBaseAllocateArr->array != NULL);
    memset(newBaseAllocateArr->array, 0, sizeof(BaseAllocate *) * member);

    return newBaseAllocateArr;
}

/**
 * @brief Destroys a BaseAllocateArr structure.
 *
 * This function deallocates the memory used by the BaseAllocateArr structure.
 * It first checks if the BaseAllocateArr pointer is not NULL.
 * If it is not NULL, it checks if the array of BaseAllocate pointers is not NULL.
 * If the array is not NULL, it iterates over the array and destroys each BaseAllocate.
 * It then deallocates the memory used by the array of BaseAllocate pointers.
 * Finally, it deallocates the memory used by the BaseAllocateArr structure itself.
 *
 * @param baseAllocateArr Pointer to the BaseAllocateArr structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyBaseAllocateArr(BaseAllocateArr *baseAllocateArr, Allocator *allocator) {
    if (baseAllocateArr != NULL) {
        if (baseAllocateArr->array != NULL) {
            for (int i = 0; i < baseAllocateArr->member; ++i) {
                destroyBaseAllocate(baseAllocateArr->array[i], allocator);
                baseAllocateArr->array[i] = NULL;
            }
        }

        size_t arraySize = sizeof(BaseAllocate *) * baseAllocateArr->max_member;
        allocator->deallocate(allocator, baseAllocateArr->array, arraySize);
        allocator->deallocate(allocator, baseAllocateArr, sizeof(BaseAllocateArr));
    }
}

BaseAllocateArr * deepCopyBaseAllocateArr(BaseAllocateArr* baseAllocateArr, Allocator* allocator){
    BaseAllocateArr* newBaseAllocateArr = initBaseAllocateArr(allocator, baseAllocateArr->max_member);
    for (int i = 0; i < baseAllocateArr->member; ++i) {
        newBaseAllocateArr->array[i] = initBaseAllocate(allocator, baseAllocateArr->array[i]->type, baseAllocateArr->array[i]->number);
    }
    newBaseAllocateArr->member = baseAllocateArr->member;
    return newBaseAllocateArr;
}



/**
 * @brief Initializes a BaseAllocateArr structure with resources.
 *
 * This function initializes a BaseAllocateArr structure with the provided resources.
 * If the number of resources is greater than the maximum number of members in the BaseAllocateArr, it increases the capacity of the BaseAllocateArr.
 * It then creates a new BaseAllocate for each resource and adds it to the BaseAllocateArr.
 *
 * @param destArr Pointer to the BaseAllocateArr structure to be initialized.
 * @param resourceArr 2D array of resources, where each row contains a resource type and its quantity.
 * @param rows Number of rows in the resourceArr array.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
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

/**
 * @brief Displays a BaseAllocateArr structure.
 *
 * This function prints the details of the BaseAllocateArr structure to the console.
 * It iterates over the array of BaseAllocate pointers in the BaseAllocateArr and for each BaseAllocate,
 * it prints the resource type and number in the format "[type, number]".
 *
 * @param baseAllocateArr Pointer to the BaseAllocateArr structure to be displayed.
 */
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

/**
 * @brief Adds a resource to a BaseAllocateArr structure.
 *
 * This function adds a resource to a BaseAllocateArr structure.
 * If the BaseAllocateArr is full, it increases the capacity of the BaseAllocateArr.
 * It then creates a new BaseAllocate for the resource and adds it to the BaseAllocateArr.
 *
 * @param destArr Pointer to the BaseAllocateArr structure where the resource will be added.
 * @param resource Array containing the resource type and its quantity.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void pushToResourceArr(BaseAllocateArr *destArr, ResourceType resource[2], Allocator *allocator) {
    if (destArr->member >= destArr->max_member) {
        assert(upArrCapacity(destArr, allocator) != true);
    }
    destArr->array[destArr->member++] = initBaseAllocate(allocator, resource[0], resource[1]);
}

/**
 * @brief Adds a BaseAllocate to a BaseAllocateArr structure.
 *
 * This function adds a BaseAllocate to a BaseAllocateArr structure.
 * It creates a new resource with the type and number from the provided BaseAllocate and adds it to the BaseAllocateArr.
 *
 * @param destArr Pointer to the BaseAllocateArr structure where the BaseAllocate will be added.
 * @param resource Pointer to the BaseAllocate to be added.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void pushToResourceArrUsedBaseAllocate(BaseAllocateArr *destArr, BaseAllocate *resource, Allocator *allocator) {
    ResourceType resourceArr[2] = {resource->type, resource->number};
    pushToResourceArr(destArr, resourceArr, allocator);
}

/**
 * @brief Increases the capacity of a BaseAllocateArr structure.
 *
 * This function increases the capacity of a BaseAllocateArr structure.
 * It calculates the new capacity, which is twice the current capacity, unless the current capacity is less than 10, in which case the new capacity is set to 10.
 * It then reallocates memory for the array of BaseAllocate pointers in the BaseAllocateArr to increase its capacity.
 * If the reallocation is successful, the function updates the max_member field of the BaseAllocateArr with the new capacity and returns true.
 * If the reallocation fails, the function returns false.
 *
 * @param destArr Pointer to the BaseAllocateArr structure whose capacity is to be increased.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Boolean value indicating whether the capacity increase was successful.
 */
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

/**
 * @brief Initializes an AllocatorResource structure.
 *
 * This function allocates memory for a new AllocatorResource structure and initializes its fields.
 * It sets the size to DEFAULT_BASE_ALLOCATE and initializes the maxResource, assignedResource, and needResource arrays.
 * Each of these arrays is initialized by calling the initBaseAllocateArr function with the allocator and the size of the new AllocatorResource.
 *
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created AllocatorResource structure.
 */
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
