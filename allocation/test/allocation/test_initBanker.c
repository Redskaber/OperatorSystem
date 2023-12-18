/*
 User: Redskaber
 Date: 2023/12/18
 Time: 9:45
*/
#include "../header/test_initBanker.h"


void test_initBanker_withValidInputs_initializesBankerCorrectly() {
    // Initialize a SystemResource structure.
    SystemResource *systemResource = initSystemResource(500, 100, 100, 100, 100, 100);

    // Initialize a Banker structure with some available resources.
    ResourceType availableResourceArr[][2] = {
            {cpu,    10},
            {memory, 20},
            {swap,   30}
    };
    Banker *banker = initBanker(availableResourceArr, 3, systemResource);

    // Check if the Banker structure is initialized correctly.
    assert(banker != NULL);
    assert(banker->size == 0);
    assert(banker->maxSize == BANKER_INIT_ARRAY_MEMBER);
    assert(banker->array != NULL);
    assert(banker->availableResource != NULL);

    // Clean up.
    destroyBanker(banker, systemResource);
    destroySystemResource(systemResource);
}

void test_initBanker_withEmptyResourceArray_initializesBankerCorrectly() {
    // Initialize a SystemResource structure.
    SystemResource *systemResource = initSystemResource(500, 100, 100, 100, 100, 100);

    // Initialize a Banker structure with an empty resource array.
    ResourceType availableResourceArr[][2] = {};
    Banker *banker = initBanker(availableResourceArr, 0, systemResource);

    // Check if the Banker structure is initialized correctly.
    assert(banker != NULL);
    assert(banker->size == 0);
    assert(banker->maxSize == BANKER_INIT_ARRAY_MEMBER);
    assert(banker->array != NULL);
    assert(banker->availableResource != NULL);

    // Clean up.
    destroyBanker(banker, systemResource);
    destroySystemResource(systemResource);
}

void test_initBanker_withNegativeNumberRows_initializesBankerCorrectly() {
    // Initialize a SystemResource structure.
    SystemResource *systemResource = initSystemResource(500, 100, 100, 100, 100, 100);

    // Initialize a Banker structure with an empty resource array.
    ResourceType availableResourceArr[][2] = {};
    Banker *banker = initBanker(availableResourceArr, -2, systemResource);

    // Check if the Banker structure is initialized correctly.
    assert(banker != NULL);
    assert(banker->size == 0);
    assert(banker->maxSize == BANKER_INIT_ARRAY_MEMBER);
    assert(banker->array != NULL);
    assert(banker->availableResource != NULL);

    // Clean up.
    destroyBanker(banker, systemResource);
    destroySystemResource(systemResource);
}

