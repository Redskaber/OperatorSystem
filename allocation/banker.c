/*
 User: Redskaber
 Date: 2023/12/7
 Time: 15:13
*/
#include "banker.h"

static void initHashMap(HashMap *map, BaseAllocateArr *availableResource);

static _Bool canBeAllocated(BaseAllocateArr *needResource, BaseAllocateArr *availableResource);

static BankProConBlock *deepCopyBankProConBlock(BankProConBlock *bankProConBlock, SystemResource *systemResource);

static void deepCopyAllocatorResource(
        AllocatorResource *destResource,
        AllocatorResource *sourceResource,
        SystemResource *systemResource
);


/**
 * @brief Initializes the AllocatorResource array in the BankProConBlock structure.
 *
 * This function initializes the maxResource and assignedResource arrays in the AllocatorResource structure
 * of the BankProConBlock. It also calculates the needResource array by subtracting the assignedResource from
 * the maxResource for each resource type. The needResource array is then initialized with these calculated values.
 *
 * @param bankProConBlock Pointer to the BankProConBlock structure.
 * @param maxResourceArr 2D array of maximum resources for each type.
 * @param assignedResourceArr 2D array of assigned resources for each type.
 * @param rows Number of rows in the maxResourceArr and assignedResourceArr arrays.
 * @param systemResource Pointer to the SystemResource structure.
 */
void initAllocatorResourceArr(
        BankProConBlock *bankProConBlock,
        int rows,
        ResourceType maxResourceArr[rows][2],
        ResourceType assignedResourceArr[rows][2],
        SystemResource *systemResource
) {
    initResourceArr(
            bankProConBlock->resource->maxResource, rows,
            maxResourceArr,
            systemResource->memory);
    initResourceArr(
            bankProConBlock->resource->assignedResource, rows,
            assignedResourceArr,
            systemResource->memory);

    ResourceType needResourceArr[rows][2] = {};
    for (int i = 0; i < rows; ++i) {
        needResourceArr[i][0] = maxResourceArr[i][0];
        needResourceArr[i][1] = maxResourceArr[i][1] - assignedResourceArr[i][1];
    }
    initResourceArr(
            bankProConBlock->resource->needResource, rows,
            needResourceArr,
            systemResource->memory);
}

/**
 * @brief Displays the resources in the AllocatorResource structure.
 *
 * This function prints the resources in the AllocatorResource structure to the console.
 * It displays the maxResource, assignedResource, and needResource arrays.
 * Each resource type and its corresponding number are printed in the format "type|number".
 *
 * @param allocatorResource Pointer to the AllocatorResource structure.
 */
void displayAllocatorResource(AllocatorResource *allocatorResource) {
    printf_s("###################################\n");
    printf_s("AllocatorResource:\n");

    printf_s("\tmaxResource:\n\t\t");
    BaseAllocate *temp = NULL;
    for (int i = 0; i < allocatorResource->maxResource->member; ++i) {
        temp = allocatorResource->maxResource->array[i];
        printf_s("%s|%-4d", resourceTypeToString(temp->type), temp->number);
    }
    printf_s("\n");

    printf_s("\tassignedResource:\n\t\t");
    for (int i = 0; i < allocatorResource->assignedResource->member; ++i) {
        temp = allocatorResource->assignedResource->array[i];
        printf_s("%s|%-4d", resourceTypeToString(temp->type), temp->number);
    }
    printf_s("\n");

    printf_s("\tneedResource:\n\t\t");
    for (int i = 0; i < allocatorResource->needResource->member; ++i) {
        temp = allocatorResource->needResource->array[i];
        printf_s("%s|%-4d", resourceTypeToString(temp->type), temp->number);
    }
    printf_s("\n");

    printf_s("###################################\n");
}

/**
 * @brief Destroys the AllocatorResource structure.
 *
 * This function deallocates the memory used by the AllocatorResource structure.
 * It first checks if the AllocatorResource pointer is not NULL.
 * If it is not NULL, it destroys the maxResource, assignedResource, and needResource arrays in the AllocatorResource structure.
 * Finally, it deallocates the memory used by the AllocatorResource structure itself.
 *
 * @param allocatorResource Pointer to the AllocatorResource structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
static void destroyAllocatorResource(AllocatorResource *allocatorResource, Allocator *allocator) {
    if (allocatorResource != NULL) {
        destroyBaseAllocateArr(allocatorResource->maxResource, allocator);
        destroyBaseAllocateArr(allocatorResource->assignedResource, allocator);
        destroyBaseAllocateArr(allocatorResource->needResource, allocator);

        allocator->deallocate(allocator, allocatorResource, sizeof(AllocatorResource));
    }
}

/**
 * @brief Initializes a BankProConBlock structure.
 *
 * This function allocates memory for a new BankProConBlock structure and initializes its fields.
 * It assigns the provided process ID, name, total time, priority, and callback function to the ProConBlock base structure.
 * It also initializes the AllocatorResource structure of the BankProConBlock.
 *
 * @param p_id The process ID to be assigned to the ProConBlock.
 * @param p_name The process name to be assigned to the ProConBlock.
 * @param p_total_time The total time to be assigned to the ProConBlock.
 * @param p_priority The priority to be assigned to the ProConBlock.
 * @param callBack The callback function to be assigned to the ProConBlock.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created BankProConBlock structure.
 */
BankProConBlock *initBankProConBlock(
        int p_id,
        char *p_name,
        double p_total_time,
        ProcessPriority p_priority,
        CallBack callBack,
        Allocator *allocator
) {
    BankProConBlock *newBankProConBlock = allocator->allocate(allocator, sizeof(BankProConBlock));  // 2296 16
    assert(newBankProConBlock != NULL);
    newBankProConBlock->base = initProConBlock(p_id, p_name, p_total_time, p_priority, callBack, allocator);
    newBankProConBlock->resource = initAllocatorResource(allocator);

    return newBankProConBlock;
}

/**
 * @brief Initializes a BankProConBlock structure with an existing ProConBlock.
 *
 * This function allocates memory for a new BankProConBlock structure and initializes its fields.
 * It assigns the provided ProConBlock to the base structure of the BankProConBlock.
 * It also initializes the AllocatorResource structure of the BankProConBlock.
 *
 * @param proConBlock Pointer to the existing ProConBlock structure.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created BankProConBlock structure.
 */
BankProConBlock *initBankProConBlockUsed(ProConBlock *proConBlock, Allocator *allocator) {
    BankProConBlock *newBankProConBlock = allocator->allocate(allocator, sizeof(BankProConBlock));
    assert(newBankProConBlock != NULL);
    newBankProConBlock->base = proConBlock;
    newBankProConBlock->resource = initAllocatorResource(allocator);

    return newBankProConBlock;
}

/**
 * @brief Displays the BankProConBlock structure.
 *
 * This function prints the details of the BankProConBlock structure to the console.
 * It displays the ProConBlock base structure and the AllocatorResource structure.
 *
 * @param bankProConBlock Pointer to the BankProConBlock structure.
 */
void displayBankProConBlock(BankProConBlock *bankProConBlock) {
    displayProConBlock(bankProConBlock->base);
    displayAllocatorResource(bankProConBlock->resource);
}

/**
 * @brief Destroys the BankProConBlock structure.
 *
 * This function deallocates the memory used by the BankProConBlock structure.
 * It first checks if the BankProConBlock pointer is not NULL.
 * If it is not NULL, it destroys the ProConBlock base structure and the AllocatorResource structure in the BankProConBlock.
 * Finally, it deallocates the memory used by the BankProConBlock structure itself.
 *
 * @param bankProConBlock Pointer to the BankProConBlock structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyBankProConBlock(BankProConBlock *bankProConBlock, Allocator *allocator) {
    if (bankProConBlock != NULL) {
        destroyProConBlock(bankProConBlock->base, allocator);
        destroyAllocatorResource(bankProConBlock->resource, allocator);
        allocator->deallocate(allocator, bankProConBlock, sizeof(BankProConBlock));
        bankProConBlock = NULL;
    }
}

/**
 * @brief Initializes a Banker structure.
 *
 * This function allocates memory for a new Banker structure and initializes its fields.
 * It sets the size to 0 and the maxSize to BANKER_INIT_ARRAY_MEMBER.
 * It also allocates memory for the array of BankProConBlock pointers and initializes the availableResource array.
 * The availableResource array is initialized with the provided availableResourceArr 2D array.
 *
 * @param availableResourceArr 2D array of available resources.
 * @param rows Number of rows in the availableResourceArr array.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 * @return Pointer to the newly created Banker structure.
 */
Banker *initBanker(
        ResourceType availableResourceArr[][2],
        int rows,
        SystemResource *systemResource
) {
    Banker *newBanker = NULL;
    assert(rows >= 0);

    newBanker = systemResource->memory->allocate(systemResource->memory, sizeof(Banker));
    assert(newBanker != NULL);

    newBanker->size = 0;
    newBanker->maxSize = BANKER_INIT_ARRAY_MEMBER;
    size_t initSize = newBanker->maxSize * sizeof(BankProConBlock *);
    newBanker->array = systemResource->memory->allocate(systemResource->memory, initSize);
    assert(newBanker->array != NULL);

    newBanker->availableResource = initBaseAllocateArr(systemResource->memory, newBanker->maxSize);

    if (rows > 0) {
        initResourceArr(
                newBanker->availableResource, rows,
                availableResourceArr,
                systemResource->memory
        );
    }
    return newBanker;
}

/**
 * @brief Destroys the Banker structure.
 *
 * This function deallocates the memory used by the Banker structure.
 * It first checks if the Banker pointer is not NULL.
 * If it is not NULL, it iterates over the array of BankProConBlock pointers and destroys each BankProConBlock.
 * It then deallocates the memory used by the array of BankProConBlock pointers and the availableResource array in the Banker structure.
 * Finally, it deallocates the memory used by the Banker structure itself and sets the Banker pointer to NULL.
 *
 * @param banker Pointer to the Banker structure to be destroyed.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 */
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

/**
 * @brief Increases the capacity of the BankProConBlock array in the Banker structure.
 *
 * This function reallocates memory for the BankProConBlock array in the Banker structure to increase its capacity.
 * The new capacity is twice the current capacity, unless the current capacity is less than 5, in which case the new capacity is set to 5.
 * If the reallocation is successful, the function updates the maxSize field of the Banker structure and returns true.
 * If the reallocation fails, the function returns false.
 *
 * @param banker Pointer to the Banker structure whose BankProConBlock array capacity is to be increased.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 * @return Boolean value indicating whether the capacity increase was successful.
 */
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

/**
 * @brief Adds a BankProConBlock to the Banker structure.
 *
 * This function adds a BankProConBlock to the array of BankProConBlock pointers in the Banker structure.
 * If the size of the array is equal to or greater than its maximum size, the function increases the capacity of the array.
 * The BankProConBlock is then added to the end of the array and the size of the array is incremented.
 *
 * @param banker Pointer to the Banker structure to which the BankProConBlock is to be added.
 * @param bankProConBlock Pointer to the BankProConBlock to be added.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 */
void pushProConBlockToBanker(Banker *banker, BankProConBlock *bankProConBlock, SystemResource *systemResource) {
    if (banker->size >= banker->maxSize) {
        assert(upCapacityBankerProConBlockArr(banker, systemResource) == true);
    }
    banker->array[banker->size++] = bankProConBlock;
}

/**
 * @brief Removes a BankProConBlock from the Banker structure and destroys it.
 *
 * This function removes a BankProConBlock from the array of BankProConBlock pointers in the Banker structure.
 * It first finds the index of the BankProConBlock in the array. If the BankProConBlock is not found, the function returns without doing anything.
 * Otherwise, it shifts all BankProConBlocks after the removed one to fill the gap left by the removed BankProConBlock.
 * It then decrements the size of the array and destroys the removed BankProConBlock.
 *
 * @param banker Pointer to the Banker structure from which the BankProConBlock is to be removed.
 * @param bankProConBlock Pointer to the BankProConBlock to be removed.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 */
void removeBankProConBlockFromBanker(Banker *banker, BankProConBlock *bankProConBlock, SystemResource *systemResource) {
    int index = -1;
    for (int i = 0; i < banker->size; ++i) {
        if (banker->array[i] == bankProConBlock) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }

    for (int i = index; i < banker->size - 1; ++i) {
        banker->array[i] = banker->array[i + 1];
    }
    banker->size -= 1;
    destroyBankProConBlock(bankProConBlock, systemResource->memory);
}

/**
 * @brief Initializes a HashMapResource with the available resources.
 *
 * This function iterates over the available resources and inserts each one into the HashMapResource.
 * The resource type is used as the key and the address of the quantity of the resource is used as the value.
 *
 * @param mapResource Pointer to the HashMapResource to be initialized.
 * @param availableResource Pointer to the BaseAllocateArr structure representing the available resources.
 */
static void initHashMapResource(HashMapResource *mapResource, BaseAllocateArr *availableResource) {
    for (int i = 0; i < availableResource->member; ++i) {
        char *key = resourceTypeToString(availableResource->array[i]->type);
        int *value = &availableResource->array[i]->number;
        insertResource(mapResource, key, value);
    }
}

/**
 * @brief Checks whether resources can be allocated.
 *
 * This function checks if the resources requested by a process can be allocated from the available resources.
 * It creates a HashMapResource from the available resources, where each resource type is a key and its quantity is the value.
 * It then iterates over the requested resources. For each requested resource, it checks if the resource is available in sufficient quantity.
 * If the resource is available, it subtracts the requested quantity from the available quantity.
 * If the resource is not available in sufficient quantity, it rolls back the changes made to the available resources and returns false.
 * If all requested resources can be allocated, it returns true.
 *
 * @param rows The number of resource types.
 * @param assignedResourceArr 2D array of requested resources, where each row contains a resource type and its requested quantity.
 * @param availableResource Pointer to the BaseAllocateArr structure representing the available resources.
 * @return Boolean value indicating whether the resources can be allocated.
 */
static _Bool whetherResourcesCanBeAllocated(
        int rows,
        ResourceType assignedResourceArr[rows][2],
        BaseAllocateArr *availableResource
) {
    HashMapResource *mapResource = createHashMapResource(availableResource->member);
    initHashMapResource(mapResource, availableResource);

    char *key = NULL;
    int *value = NULL;
    _Bool flag = true;
    for (int i = 0; i < rows; ++i) {
        key = resourceTypeToString(assignedResourceArr[i][0]);
        value = getResource(mapResource, key);
        if (value != NULL && assignedResourceArr[i][1] <= *value) {
            *value -= (int) assignedResourceArr[i][1];
        } else {
            for (int j = i - 1; j >= 0; --j) {
                key = resourceTypeToString(assignedResourceArr[j][0]);
                value = getResource(mapResource, key);
                *value += (int) assignedResourceArr[j][1];
            }
            flag = false;
            break;
        }
    }

    destroyHashMapResource(mapResource);
    return flag;
}

/**
 * @brief Adds an array of ProConBlocks to the Banker structure.
 *
 * This function iterates over an array of ProConBlocks and adds each one to the Banker structure.
 * For each ProConBlock, it creates a BankProConBlock and initializes its AllocatorResource structure with the corresponding resources from the bankerProConBlockGroup array.
 * It then checks if the resources can be allocated from the available resources in the Banker structure.
 * If the resources can be allocated, it initializes the AllocatorResource structure of the BankProConBlock and adds the BankProConBlock to the Banker structure.
 *
 * @param banker Pointer to the Banker structure to which the ProConBlocks are to be added.
 * @param pcbArr Array of pointers to the ProConBlocks to be added.
 * @param member The number of ProConBlocks in the pcbArr array.
 * @param rows The number of resource types.
 * @param bankerProConBlockGroup 3D array of resources for each ProConBlock, where each row contains a resource type, its maximum quantity, and its assigned quantity.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 */
void pushProConBlockArrToBanker(
        Banker *banker,
        ProConBlock **pcbArr,
        int member, int rows,
        ResourceType bankerProConBlockGroup[member][rows][3],
        SystemResource *systemResource
) {
    BankProConBlock *bankProConBlock = NULL;

    // warning:C99 forbids variable length array 'needResourceArr' [-Wvla]
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

        _Bool canAllocated = whetherResourcesCanBeAllocated(rows, assignedResourceArr, banker->availableResource);
        if (canAllocated == false) {
            displayBaseAllocateArr(banker->availableResource);
            displayResourceTypArr(member, assignedResourceArr);
            assert(canAllocated == true);
        }

        initAllocatorResourceArr(bankProConBlock, rows, maxResourceArr, assignedResourceArr, systemResource);
        pushProConBlockToBanker(banker, bankProConBlock, systemResource);
    }
}

/**
 * @brief Initializes a HashMap with the available resources.
 *
 * This function iterates over the available resources and inserts each one into the HashMap.
 * The resource type is used as the key and the quantity of the resource is used as the value.
 *
 * @param map Pointer to the HashMap to be initialized.
 * @param availableResource Pointer to the BaseAllocateArr structure representing the available resources.
 */
static void initHashMap(HashMap *map, BaseAllocateArr *availableResource) {
    for (int i = 0; i < availableResource->member; ++i) {
        char *key = resourceTypeToString(availableResource->array[i]->type);
        int value = availableResource->array[i]->number;
        insert(map, key, value);
    }
}

/**
 * @brief Initializes an array of BaseAllocateArr structures with requested resources.
 *
 * This function creates an array of BaseAllocateArr structures and initializes each one with the requested resources for a process.
 * It first allocates memory for the array of BaseAllocateArr pointers. Then, for each process, it creates a new BaseAllocateArr structure and initializes it with the requested resources.
 * The requested resources are provided as a 3D array, where each row corresponds to a process and contains a 2D array of resource types and their requested quantities.
 * The function iterates over this 3D array and, for each process, it pushes the requested resources to the corresponding BaseAllocateArr structure.
 *
 * @param mp The number of processes.
 * @param mr The number of resource types.
 * @param requestResource 3D array of requested resources, where each row contains a 2D array of resource types and their requested quantities.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created array of BaseAllocateArr structures.
 */
static BaseAllocateArr **
initRequestResourceArr(int mp, int mr, ResourceType requestResource[mp][mr][2], Allocator *allocator) {
    BaseAllocateArr **requestResourceArr = allocator->allocate(allocator, sizeof(BaseAllocateArr *) * mp);
    for (int i = 0; i < mp; ++i) {
        requestResourceArr[i] = initBaseAllocateArr(allocator, mr);
        for (int j = 0; j < mr; ++j) {
            pushToBaseAllocateArrByType(
                    requestResourceArr[i],
                    requestResource[i][j][0],
                    requestResource[i][j][1],
                    allocator
            );
        }
    }
    return requestResourceArr;
}

/**
 * @brief Checks whether resources can be allocated.
 *
 * This function checks if the resources requested by a process can be allocated from the available resources.
 * It creates a HashMap from the available resources, where each resource type is a key and its quantity is the value.
 * It then iterates over the requested resources. For each requested resource, it checks if the resource is available in sufficient quantity.
 * If the resource is available, it increments a counter.
 * If all requested resources can be allocated (i.e., the counter equals the number of requested resources), it returns true.
 * Otherwise, it returns false.
 *
 * @param needResource Pointer to the BaseAllocateArr structure representing the requested resources.
 * @param availableResource Pointer to the BaseAllocateArr structure representing the available resources.
 * @return Boolean value indicating whether the resources can be allocated.
 */
static _Bool canBeAllocated(BaseAllocateArr *needResource, BaseAllocateArr *availableResource) {
    HashMap *map = createHashMap(availableResource->member);
    initHashMap(map, availableResource);

    int counter = 0;
    for (int i = 0; i < needResource->member; ++i) {
        char *key = resourceTypeToString(needResource->array[i]->type);
        int resource = get(map, key);
        if (resource != -1 && needResource->array[i]->number <= resource) {
            counter += 1;
        }
    }
    destroyHashMap(map);
    return counter == needResource->member;
}

/**
 * @brief Simulates the allocation of resources to a process.
 *
 * This function simulates the allocation of resources to a process during its execution.
 * It creates a HashMapResource from the available resources, where each resource type is a key and its quantity is the value.
 * It then iterates over the needed resources of the process. For each needed resource, it checks if the resource is available.
 * If the resource is available, it adds the needed quantity to the assigned resources of the process.
 * After all resources have been allocated, it destroys the HashMapResource.
 *
 * @param availableResource Pointer to the BaseAllocateArr structure representing the available resources.
 * @param processResource Pointer to the AllocatorResource structure representing the resources of the process.
 */
static void
simulatedAllocatedResourceToProcess(BaseAllocateArr *availableResource, AllocatorResource *processResource) {
    HashMapResource *mapResource = createHashMapResource(availableResource->member);
    initHashMapResource(mapResource, availableResource);
    for (int i = 0; i < processResource->needResource->member; ++i) {
        char *key = resourceTypeToString(processResource->needResource->array[i]->type);
        int *value = getResource(mapResource, key);
        *value += processResource->assignedResource->array[i]->number;
    }
    destroyHashMapResource(mapResource);
}

/**
 * @brief Simulates the request of resources by a process.
 *
 * This function simulates the request of resources by a process during its execution.
 * It creates a HashMapResource from the available resources, where each resource type is a key and its quantity is the value.
 * It then iterates over the needed resources of the process. For each needed resource, it checks if the resource is available.
 * If the resource is available, it subtracts the needed quantity from the available resources and adds it to the assigned resources of the process.
 * It also subtracts the needed quantity from the needed resources of the process.
 * After all resources have been requested, it destroys the HashMapResource.
 *
 * @param availableResource Pointer to the BaseAllocateArr structure representing the available resources.
 * @param processResource Pointer to the AllocatorResource structure representing the resources of the process.
 */
static void
simulatedRequestResourceToProcess(BaseAllocateArr *availableResource, AllocatorResource *processResource) {
    HashMapResource *mapResource = createHashMapResource(availableResource->member);
    initHashMapResource(mapResource, availableResource);
    for (int i = 0; i < processResource->needResource->member; ++i) {
        // check if the requested resources can be allocated
        char *key = resourceTypeToString(processResource->needResource->array[i]->type);
        int *value = getResource(mapResource, key);
        *value -= processResource->needResource->array[i]->number;
        // add the requested resources to the assigned resources and div need Resource
        processResource->assignedResource->array[i]->number += processResource->needResource->array[i]->number;
        processResource->needResource->array[i]->number -= processResource->needResource->array[i]->number;
    }
    destroyHashMapResource(mapResource);
}

/**
 * @brief Simulates the release of resources by a process.
 *
 * This function simulates the release of resources by a process after it has finished executing.
 * It creates a HashMapResource from the available resources, where each resource type is a key and its quantity is the value.
 * It then iterates over the maximum resources of the process. For each resource, it checks if the resource is available.
 * If the resource is available, it adds the quantity of the resource to the available resources and subtracts it from the assigned resources of the process.
 * It also adds the quantity of the resource to the needed resources of the process.
 * After all resources have been released, it destroys the HashMapResource.
 *
 * @param availableResource Pointer to the BaseAllocateArr structure representing the available resources.
 * @param processResource Pointer to the AllocatorResource structure representing the resources of the process.
 */
static void
simulatedReleaseResourceToProcess(BaseAllocateArr *availableResource, AllocatorResource *processResource) {
    HashMapResource *mapResource = createHashMapResource(availableResource->member);
    initHashMapResource(mapResource, availableResource);
    for (int i = 0; i < processResource->maxResource->member; ++i) {
        // check if the requested resources can be allocated
        char *key = resourceTypeToString(processResource->maxResource->array[i]->type);
        int *value = getResource(mapResource, key);
        *value += processResource->maxResource->array[i]->number;
        // div the requested resources to the assigned resources and add need Resource
        processResource->assignedResource->array[i]->number -= processResource->maxResource->array[i]->number;
        processResource->needResource->array[i]->number += processResource->maxResource->array[i]->number;
    }
    destroyHashMapResource(mapResource);
}

/**
 * @brief Displays the resources needed by a process and the available resources.
 *
 * This function prints the resources needed by a process and the available resources to the console.
 * It first prints the process ID and then calls the displayAllocatorResource function to print the resources needed by the process.
 * It then calls the displayBaseAllocateArr function to print the available resources.
 *
 * @param i The process ID.
 * @param resource Pointer to the AllocatorResource structure representing the resources needed by the process.
 * @param availableResource Pointer to the BaseAllocateArr structure representing the available resources.
 */
static void displaySafeResource(int i, AllocatorResource *resource, BaseAllocateArr *availableResource) {
    printf_s("################################################\n");
    printf_s("Process %d is executing\n", i);
    printf_s("NeedResource: \n");
    displayAllocatorResource(resource);
    printf_s("AvailableResource: \n");
    displayBaseAllocateArr(availableResource);
    printf_s("End Process\n");
    printf_s("################################################\n");
}

/**
 * @brief Displays the safe sequence of processes.
 *
 * This function prints the safe sequence of processes to the console. The safe sequence is a sequence of processes
 * that can finish without leading to a deadlock. The process IDs of the processes in the safe sequence are printed
 * in the order they appear in the sequence.
 *
 * @param size The number of processes in the safe sequence.
 * @param safeSequence Array of pointers to the BankProConBlock structures representing the safe sequence of processes.
 */
static void displaySafeSequence(int size, BankProConBlock *safeSequence[size]) {
    printf_s("################################################\n");
    printf_s("Safe Sequence: \n process id: ");
    for (int i = 0; i < size; ++i) {
        printf_s("%d ", safeSequence[i]->base->p_id);
    }
    printf_s("\n");
    printf_s("################################################\n");
}

/**
 * @brief Saves the safe sequence of processes to the orderExecute array.
 *
 * This function copies the safe sequence of processes from the safeSequence array to the orderExecute array.
 * The safe sequence is a sequence of processes that can finish without leading to a deadlock.
 * If the orderExecute array is NULL, the function returns without doing anything.
 * Otherwise, it first sets all elements of the orderExecute array to 0.
 * It then iterates over the safeSequence array and copies each process to the corresponding position in the orderExecute array.
 *
 * @param size The number of processes in the safeSequence and orderExecute arrays.
 * @param orderExecute Pointer to the array where the safe sequence will be stored.
 * @param safeSequence Array of pointers to the BankProConBlock structures representing the safe sequence of processes.
 */
static void
saveSafeSequenceToOrderExecute(int size, BankProConBlock *(*orderExecute)[size], BankProConBlock *safeSequence[size]) {
    if (orderExecute == NULL)
        return;

    memset(orderExecute, 0, size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        (*orderExecute)[i] = safeSequence[i];
    }
}

/**
 * @brief Checks if the system is in a safe state by simulating resource allocation.
 *
 * This function simulates the allocation of resources to processes to check if the system is in a safe state.
 * It uses the Banker's algorithm to avoid deadlock. The function creates a deep copy of the available resources and
 * an array to keep track of the finished processes. It then tries to find a sequence of processes that can finish
 * without leading to a deadlock. If such a sequence is found, the system is in a safe state.
 *
 * @param banker Pointer to the Banker structure representing the system state.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 * @return Boolean value indicating whether the system is in a safe state.
 */
_Bool
checkResourceSecurity(
        Banker *banker,
        SystemResource *systemResource,
        BankProConBlock *(*orderExecute)[banker->size]
) {

    BaseAllocateArr *availableResource = deepCopyBaseAllocateArr(
            banker->availableResource,
            systemResource->memory
    );
    _Bool finishArr[banker->size];
    memset(finishArr, false, banker->size * sizeof(_Bool));

    BankProConBlock *safeSequence[banker->size];
    int count = 0;

    _Bool flag = true;
    while (count < banker->size) {

        bool foundExecute = false;
        for (int i = 0; i < banker->size; ++i) {
            // find a process that has not been executed
            if (finishArr[i] == true) {
                continue;
            }
            // find a process that needs fewer resources than the available resources
            if (canBeAllocated(banker->array[i]->resource->needResource, availableResource)) {
                // Simulate the allocation of resources to the process
                simulatedAllocatedResourceToProcess(
                        availableResource,
                        banker->array[i]->resource
                );
                // Mark the process as finished and add to safe sequence, mark found process
                finishArr[i] = true;
                safeSequence[count++] = banker->array[i];
                foundExecute = true;
            }
        }
        // If no process was found, the system is not in a safe state.
        if (foundExecute == false) {
            flag = false;
            break;
        }
    }

    if (flag == true) {
        saveSafeSequenceToOrderExecute(banker->size, orderExecute, safeSequence);
    }
    destroyBaseAllocateArr(availableResource, systemResource->memory);

    return flag;
}

/**
 * @brief Displays the Banker structure.
 *
 * This function prints the details of the Banker structure to the console.
 * It displays the size, maxSize, array of BankProConBlocks, and available resources.
 * For each BankProConBlock in the array, it calls the displayBankProConBlock function to print its details.
 * For the available resources, it calls the displayBaseAllocateArr function to print its details.
 *
 * @param banker Pointer to the Banker structure to be displayed.
 */
void displayBanker(Banker *banker) {
    printf_s("###################################\n");
    printf_s("Banker:\n");
    printf_s("\tsize: %d\n", banker->size);
    printf_s("\tmaxSize: %d\n", banker->maxSize);
    printf_s("\tarray:\n");
    for (int i = 0; i < banker->size; ++i) {
        displayBankProConBlock(banker->array[i]);
    }
    printf_s("\tavailableResource:\n");
    displayBaseAllocateArr(banker->availableResource);
    printf_s("###################################\n");
}

/**
 * @brief Implements the Banker's Algorithm for resource allocation.
 *
 * This function implements the Banker's Algorithm, which is a deadlock avoidance algorithm used in operating systems to prevent deadlock.
 * It does this by denying or postponing the requests for resources from processes in the system if it determines that accepting the request could put the system in an unsafe state (a state that could lead to deadlock).
 *
 * @param mp The number of processes.
 * @param pcbArr Array of pointers to the ProConBlock structures representing the processes.
 * @param mr The number of resource types.
 * @param availableResourceArr 2D array of available resources, where each row contains a resource type and its quantity.
 * @param bankerPCBResourceGroup 3D array of resources for each process, where each row corresponds to a process and contains a 2D array of resource types, their maximum quantities, and their assigned quantities.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 */
void bankerResourceAllocatedAlgorithm(
        int mp, ProConBlock *pcbArr[mp],
        int mr, ResourceType availableResourceArr[mr][2],
        ResourceType bankerPCBResourceGroup[mp][mr][3],
        SystemResource *systemResource
) {
    displaySystemResource(systemResource);

    // Initialize resources and process information
    Banker *banker = initBanker(availableResourceArr, mr, systemResource);
    pushProConBlockArrToBanker(banker, pcbArr, mp, mr, bankerPCBResourceGroup, systemResource);

    // Design data structure for Banker's Algorithm
    BankProConBlock *orderExecute[banker->size];

    // Check if the system is in a safe state
    if (checkResourceSecurity(banker, systemResource, &orderExecute) == true) {

        // Execute according to the safe sequence, handle resource requests and allocation & release
        _Bool destroyArr[banker->size];
        memset(destroyArr, false, banker->size * sizeof(_Bool));

        for (int i = 0; i < mp; ++i) {
            // find a process that has not been executed
            if (destroyArr[i] == true) {
                continue;
            }
            BankProConBlock *safeBank = orderExecute[0];
            displaySafeSequence(banker->size, orderExecute);
            // request resource to allocated
            simulatedRequestResourceToProcess(banker->availableResource, safeBank->resource);
            // used pcb execute algorithm execute pcb callback function
            if (safeBank->base->callback != NULL) {
                safeBank->base->callback(safeBank->base);
            }
            // release resource to available
            simulatedReleaseResourceToProcess(banker->availableResource, safeBank->resource);
            // banker remove safeBank and destroy safeBank
            removeBankProConBlockFromBanker(banker, safeBank, systemResource);
            destroyArr[i] = true;
            // check whether the resource can be allocated
            assert(checkResourceSecurity(banker, systemResource, &orderExecute) == true);
        }

        // destroy array is not all true
        _Bool raiseError = false;
        for (int i = 0; i < banker->size; ++i) {
            if (destroyArr[i] == false) {
                destroyBankProConBlock(orderExecute[i], systemResource->memory);
                raiseError = true;
            }
        }
        assert(raiseError == false);
    }
    destroyBanker(banker, systemResource);
}

/**
 * @brief Creates a deep copy of a BankProConBlock structure.
 *
 * This function creates a new BankProConBlock structure and initializes it with the values from the provided BankProConBlock.
 * It uses the initBankProConBlock function to create the new BankProConBlock with the process ID, name, total time, priority, and callback function from the provided BankProConBlock.
 * It then uses the deepCopyAllocatorResource function to create a deep copy of the AllocatorResource structure from the provided BankProConBlock and assigns it to the new BankProConBlock.
 *
 * @param bankProConBlock Pointer to the BankProConBlock structure to be copied.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 * @return Pointer to the newly created BankProConBlock structure.
 */
static BankProConBlock *deepCopyBankProConBlock(BankProConBlock *bankProConBlock, SystemResource *systemResource) {
    BankProConBlock *newBankProConBlock = initBankProConBlock(
            bankProConBlock->base->p_id,
            bankProConBlock->base->p_name,
            bankProConBlock->base->p_total_time,
            bankProConBlock->base->p_priority,
            bankProConBlock->base->callback,
            systemResource->memory
    );
    deepCopyAllocatorResource(
            newBankProConBlock->resource,
            bankProConBlock->resource,
            systemResource
    );
    return newBankProConBlock;
}

/**
 * @brief Creates a deep copy of an AllocatorResource structure.
 *
 * This function creates a deep copy of the provided AllocatorResource structure.
 * It iterates over the maxResource, assignedResource, and needResource arrays in the source AllocatorResource structure.
 * For each resource in these arrays, it creates a copy of the resource and adds it to the corresponding array in the destination AllocatorResource structure.
 *
 * @param destResource Pointer to the AllocatorResource structure where the copied resources will be stored.
 * @param sourceResource Pointer to the AllocatorResource structure to be copied.
 * @param systemResource Pointer to the SystemResource structure used for memory management.
 */
static void deepCopyAllocatorResource(
        AllocatorResource *destResource,
        AllocatorResource *sourceResource,
        SystemResource *systemResource
) {
    for (int i = 0; i < sourceResource->maxResource->member; ++i) {
        pushToResourceArrUsedBaseAllocate(
                destResource->maxResource,
                sourceResource->maxResource->array[i],
                systemResource->memory
        );
        pushToResourceArrUsedBaseAllocate(
                destResource->assignedResource,
                sourceResource->assignedResource->array[i],
                systemResource->memory
        );
        pushToResourceArrUsedBaseAllocate(
                destResource->needResource,
                sourceResource->needResource->array[i],
                systemResource->memory
        );
    }
}
