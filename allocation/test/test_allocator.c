/*
 User: Redskaber
 Date: 2023/12/7
 Time: 15:18
*/

#include "test_allocator.h"

static void *proCallBack(void *args) {
    printf_s("args", args);
    return args;
}


static SystemResource *createSystemResource(Allocator *allocator) {
    SystemResource *sr = initSystemResource(ALLOCATE_TOTAL_SIZE, 1000, 1000, 200, 100, 500, allocator);
    return sr;
}


void test_allocator() {
    Allocator *allocator = createAllocator(ALLOCATE_TOTAL_SIZE);
    allocator->display(allocator);

    SystemResource *sr = createSystemResource(allocator);
    displaySystemResource(sr);
    allocator->display(allocator);


    ProConBlock *proConBlock = initProConBlock(
            0x0001,
            "process-0x0001",
            50,
            normal,
            proCallBack, allocator);
    BankProConBlock *bank_pcb = initBankProConBlockUsed(proConBlock, allocator);
    displayBankProConBlock(bank_pcb);

    ResourceType maxResource[4][2] = {
            {memory,  10},
            {cpu,     5},
            {gpu,     7},
            {network, 3}};
    ResourceType assignedResource[4][2] = {
            {memory,  6},
            {cpu,     3},
            {gpu,     5},
            {network, 2}};
    initAllocatorResourceArr(bank_pcb, maxResource, assignedResource, 4, allocator);
    displayBankProConBlock(bank_pcb);
    allocator->display(allocator);

    destroyBankProConBlock(bank_pcb, allocator);
    allocator->display(allocator);

    destroySystemResource(sr, allocator);
    allocator->display(allocator);

}
