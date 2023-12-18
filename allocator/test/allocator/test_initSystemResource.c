/*
 User: Redskaber
 Date: 2023/12/18
 Time: 9:49
*/
#include "../header/test_initSystemResource.h"


void test_initSystemResource_withValidInputs_initializesSystemResourceCorrectly() {
    SystemResource *systemResource = initSystemResource(500, 4, 2, 50, 10, 5);

    assert(systemResource != NULL);
    assert(systemResource->memory != NULL);
    assert(systemResource->cpu != NULL);
    assert(systemResource->gpu != NULL);
    assert(systemResource->swap != NULL);
    assert(systemResource->netWork != NULL);
    assert(systemResource->file != NULL);

    destroySystemResource(systemResource);
}

void test_initSystemResource_withZeroInputs_initializesSystemResourceCorrectly() {
    SystemResource *systemResource = initSystemResource(0, 0, 0, 0, 0, 0);

    assert(systemResource != NULL);
    assert(systemResource->memory != NULL);
    assert(systemResource->cpu != NULL);
    assert(systemResource->gpu != NULL);
    assert(systemResource->swap != NULL);
    assert(systemResource->netWork != NULL);
    assert(systemResource->file != NULL);

    destroySystemResource(systemResource);
}

void test_initSystemResource_withNegativeInputs_initializesSystemResourceCorrectly() {
    SystemResource *systemResource = initSystemResource(-100, -4, -2, -50, -10, -5);

    assert(systemResource != NULL);
    assert(systemResource->memory != NULL);
    assert(systemResource->cpu != NULL);
    assert(systemResource->gpu != NULL);
    assert(systemResource->swap != NULL);
    assert(systemResource->netWork != NULL);
    assert(systemResource->file != NULL);

    destroySystemResource(systemResource);
}
