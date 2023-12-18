/*
 User: Redskaber
 Date: 2023/12/18
 Time: 9:42
*/
#pragma once
#ifndef OPERATORSYSTEM_TEST_CHECKRESOURCESECURITY_H
#define OPERATORSYSTEM_TEST_CHECKRESOURCESECURITY_H

#include "stdbool.h"
#include <assert.h>
#include "../../banker.h"

extern void test_checkResourceSecurity_withSafeSequence_returnsTrue();

extern void test_checkResourceSecurity_withUnsafeSequence_returnsFalse();

#endif //OPERATORSYSTEM_TEST_CHECKRESOURCESECURITY_H
