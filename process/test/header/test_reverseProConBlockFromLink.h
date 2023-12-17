/*
 User: Redskaber
 Date: 2023/12/17
 Time: 17:27
*/
#ifndef OPERATORSYSTEM_TEST_REVERSEPROCONBLOCKFROMLINK_H
#define OPERATORSYSTEM_TEST_REVERSEPROCONBLOCKFROMLINK_H

#include <assert.h>
#include <stdlib.h>
#include "../../process_scheduling.h"

extern void test_reverseProConBlockFromLink_whenLinkIsEmpty_doesNotCrash();

extern void test_reverseProConBlockFromLink_whenLinkHasOneElement_doesNotChangeOrder();

extern void test_reverseProConBlockFromLink_whenLinkHasMultipleElements_reversesOrder();

#endif //OPERATORSYSTEM_TEST_REVERSEPROCONBLOCKFROMLINK_H
