/*
 User: Redskaber
 Date: 2023/12/18
 Time: 9:42
*/
#include "../header/test_checkResourceSecurity.h"


void test_checkResourceSecurity_withSafeSequence_returnsTrue() {
    // Initialize a SystemResource structure.
    SystemResource *systemResource = initSystemResource(3000, 100, 100, 100, 100, 100);

    // Initialize a Banker structure with some available resources.
    ResourceType availableResourceArr[][2] = {
            {cpu,    20},
            {memory, 25},
            {swap,   30}
    };
    Banker *banker = initBanker(availableResourceArr, 3, systemResource);

    // Add some processes to the Banker structure.
    ResourceType bankerProConBlockGroup[3][3][3] = {
            {
                    {cpu, 7, 5},
                    {memory, 10, 5},
                    {swap, 15, 10}
            },
            {
                    {cpu, 3, 2},
                    {memory, 7,  2},
                    {swap, 5,  2}
            },
            {
                    {cpu, 9, 4},
                    {memory, 5,  2},
                    {swap, 7,  4}
            }
    };
    ProConBlock *pcbArr[3] = {
            initProConBlock(1, "Process 1", 10.0, high, NULL, systemResource->memory),
            initProConBlock(2, "Process 2", 20.0, normal, NULL, systemResource->memory),
            initProConBlock(3, "Process 3", 30.0, low, NULL, systemResource->memory)
    };
    pushProConBlockArrToBanker(banker, pcbArr, 3, 3, bankerProConBlockGroup, systemResource);

    // Check if the system is in a safe state.
    int orderExecute[banker->size] = {};
    assert(checkResourceSecurity(banker, systemResource, &orderExecute) == true);
    for (int i = 0; i < banker->size; i++) {
        printf_s("%d ", orderExecute[i]);
    }
    printf_s("\n");
    // Clean up.
    destroyBanker(banker, systemResource);
    destroySystemResource(systemResource);
}

void test_checkResourceSecurity_withUnsafeSequence_returnsFalse() {
    // Initialize a SystemResource structure.
    SystemResource *systemResource = initSystemResource(3000, 100, 100, 100, 100, 100);

    // Initialize a Banker structure with some available resources.
    ResourceType availableResourceArr[][2] = {
            {cpu,    10},
            {memory, 20},
            {swap,   30}
    };
    Banker *banker = initBanker(availableResourceArr, 3, systemResource);

    // Add some processes to the Banker structure.
    ResourceType bankerProConBlockGroup[][3][3] = {
            {
                    {cpu, 7,  5},
                    {memory, 10, 5},
                    {swap, 15, 10}
            },
            {
                    {cpu, 13, 2},
                    {memory, 17, 2},
                    {swap, 25, 2}
            },
            {
                    {cpu, 9,  4},
                    {memory, 5,  2},
                    {swap, 7,  4}
            }
    };
    ProConBlock *pcbArr[] = {
            initProConBlock(1, "Process 1", 10.0, high, NULL, systemResource->memory),
            initProConBlock(2, "Process 2", 20.0, normal, NULL, systemResource->memory),
            initProConBlock(3, "Process 3", 30.0, low, NULL, systemResource->memory)
    };
    pushProConBlockArrToBanker(banker, pcbArr, 3, 3, bankerProConBlockGroup, systemResource);

    // Check if the system is in a safe state.
    int orderExecute[banker->size] = {};
    assert(checkResourceSecurity(banker, systemResource, &orderExecute) == false);
    for (int i = 0; i < banker->size; i++) {
        printf_s("%d ", orderExecute[i]);
    }
    printf_s("\n");
    // Clean up.
    destroyBanker(banker, systemResource);
    destroySystemResource(systemResource);
}
