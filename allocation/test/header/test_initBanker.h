/*
 User: Redskaber
 Date: 2023/12/18
 Time: 9:45
*/
#pragma once
#ifndef OPERATORSYSTEM_TEST_INITBANKER_H
#define OPERATORSYSTEM_TEST_INITBANKER_H

#include <assert.h>
#include "../../banker.h"

extern void test_initBanker_withValidInputs_initializesBankerCorrectly();

extern void test_initBanker_withEmptyResourceArray_initializesBankerCorrectly();

extern void test_initBanker_withNegativeNumberRows_initializesBankerCorrectly();

#endif //OPERATORSYSTEM_TEST_INITBANKER_H
