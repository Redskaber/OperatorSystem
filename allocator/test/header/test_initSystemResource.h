/*
 User: Redskaber
 Date: 2023/12/18
 Time: 9:49
*/
#ifndef OPERATORSYSTEM_TEST_INITSYSTEMRESOURCE_H
#define OPERATORSYSTEM_TEST_INITSYSTEMRESOURCE_H

#include <assert.h>
#include "../../systemResource.h"

extern void test_initSystemResource_withValidInputs_initializesSystemResourceCorrectly();

extern void test_initSystemResource_withZeroInputs_initializesSystemResourceCorrectly();

extern void test_initSystemResource_withNegativeInputs_initializesSystemResourceCorrectly();

#endif //OPERATORSYSTEM_TEST_INITSYSTEMRESOURCE_H
