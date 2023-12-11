/*
 User: Redskaber
 Date: 2023/12/7
 Time: 15:18
*/

#include "test_allocator.h"

void test_allocator() {
    Allocator *allocator = createAllocator(ALLOCATE_TOTAL_SIZE);
    allocator->display(allocator);
    SystemResource *sr = initSystemResource(1000, 1000, 1000, 200, 100, 500, allocator);
    allocator->display(allocator);
    displaySystemResource(sr);

}




