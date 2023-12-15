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
    SystemResource *sr = initSystemResource(5000, 1000, 1000, 200, 100, 500);
    return sr;
}


void test_allocator() {
    SystemResource *sr = createSystemResource();
    displaySystemResource(sr);

    ResourceType availableResource[4][2] = {
            {memory,  50},
            {cpu,     50},
            {gpu,     50},
            {network, 50}};
    Banker *banker = initBanker(availableResource, 4, sr);

    ResourceType bankerProConBlockGroup[5][4][3] = {
            {
                    {memory, 10, 6},
                    {cpu, 5, 3},
                    {gpu, 7, 5},
                    {network, 3, 2}
            },
            {
                    {memory, 8,  6},
                    {cpu, 4, 3},
                    {gpu, 7, 5},
                    {network, 3, 2}
            },
            {
                    {memory, 2,  1},
                    {cpu, 5, 3},
                    {gpu, 5, 0},
                    {network, 3, 2}
            },
            {
                    {memory, 4,  4},
                    {cpu, 4, 3},
                    {gpu, 7, 3},
                    {network, 3, 0}
            },
            {
                    {memory, 10, 3},
                    {cpu, 3, 3},
                    {gpu, 8, 5},
                    {network, 3, 2}
            },
    };

    ProConBlock *pcb1 = initProConBlock(
            0x0001,
            "process-0x0001",
            50,
            normal,
            proCallBack, sr->memory);
    ProConBlock *pcb2 = initProConBlock(
            0x0002,
            "process-0x0002",
            30,
            exigency,
            proCallBack, sr->memory);
    ProConBlock *pcb3 = initProConBlock(
            0x0003,
            "process-0x0003",
            10,
            high,
            proCallBack, sr->memory);
    ProConBlock *pcb4 = initProConBlock(
            0x0004,
            "process-0x0004",
            20,
            normal,
            proCallBack, sr->memory);
    ProConBlock *pcb5 = initProConBlock(
            0x0005,
            "process-0x0005",
            30,
            low,
            proCallBack, sr->memory);

    ProConBlock *pcbArr[5] = {pcb1, pcb2, pcb3, pcb4, pcb5};
    pushProConBlockArrToBanker(banker, pcbArr, 5, 4, bankerProConBlockGroup, sr);

    displaySystemResource(sr);
    destroyBanker(banker, sr);

    displaySystemResource(sr);
    destroySystemResource(sr);


}
