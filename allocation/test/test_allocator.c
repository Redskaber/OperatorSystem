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


static SystemResource *createSystemResource() {
    SystemResource *sr = initSystemResource(ALLOCATE_TOTAL_SIZE, 1000, 1000, 200, 100, 500);
    return sr;
}


void test_allocator() {
    SystemResource *sr = createSystemResource();
    displaySystemResource(sr);

    // create process control block
    ProConBlock *proConBlock = initProConBlock(
            0x0001,
            "process-0x0001",
            50,
            normal,
            proCallBack, sr->memory);

    // create banker process control block
    BankProConBlock *bank_pcb = initBankProConBlockUsed(proConBlock, sr->memory);

    // init banker pcb resource
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
    initAllocatorResourceArr(
            bank_pcb,
            maxResource,
            assignedResource,
            4, sr);

    // init system available resource
    ResourceType availableResource[4][2] = {
            {memory,  50},
            {cpu,     50},
            {gpu,     50},
            {network, 50}};
    Banker *banker = initBanker(availableResource, 4, sr);

    // push pbc to banker
    pushProConBlockToBanker(banker, bank_pcb);

    displaySystemResource(sr);
    destroyBanker(banker, sr);

    displaySystemResource(sr);
    destroySystemResource(sr);

}
