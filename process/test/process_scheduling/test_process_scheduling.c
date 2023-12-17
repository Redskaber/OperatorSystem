/*
 User: Redskaber
 Date: 2023/11/30
 Time: 15:27
*/
#include "../header/test_process_scheduling.h"


static Allocator *getAllocator() {
    return createAllocator(ALLOCATE_TOTAL_SIZE);
}


// 自定义排序规则
static _Bool priorityCompare(void *a, void *b) {
    return ((ProConBlock *) a)->p_priority < ((ProConBlock *) b)->p_priority;
}

static _Bool totalTimeCompare(void *a, void *b) {
    return ((ProConBlock *) a)->p_total_time < ((ProConBlock *) b)->p_total_time;
}


static void *callback(void *proConBlock) {
//    va_list argList;
//    va_start(argList, args);
//    va_end(argList);
    printf_s("\t\tSub Process Start...\n");
    printf_s("\t\tSub Process per1...\n");
    printf_s("\t\tSub Process per2...\n");
    printf_s("\t\tSub Process per3...\n");
    printf_s("\t\tSub Process End...\n");
    return proConBlock;
}


void test_process_scheduling() {
    Allocator *allocator = getAllocator();
    allocator->display(allocator);

    ProConBlock *proConBlock1 = initProConBlock(
            0x0001,
            "process-0x0001",
            50,
            exigency,
            callback, allocator);
    ProConBlock *proConBlock2 = initProConBlock(
            0x0002,
            "process-0x0002",
            10,
            low,
            callback, allocator);
    ProConBlock *proConBlock3 = initProConBlock(
            0x0003,
            "process-0x0003",
            30,
            normal,
            callback, allocator);
    ProConBlock *proConBlock4 = initProConBlock(
            0x0004,
            "process-0x0004",
            50,
            high,
            callback, allocator);
    ProConBlock *proConBlock5 = initProConBlock(
            0x0005,
            "process-0x0005",
            20,
            exigency,
            callback, allocator);

    ProConBlockLink *proConBlockLink = initProConBlockLink(allocator);
    insertToLinkFromParam(proConBlockLink, proConBlock1, priorityCompare);
    insertToLinkFromParam(proConBlockLink, proConBlock2, priorityCompare);
    insertToLinkFromParam(proConBlockLink, proConBlock3, priorityCompare);
    insertToLinkFromParam(proConBlockLink, proConBlock4, priorityCompare);
    insertToLinkFromParam(proConBlockLink, proConBlock5, priorityCompare);
    displayProConBlockLink(proConBlockLink);

    allocator->display(allocator);
    popFrontFromLink(proConBlockLink, allocator);
    allocator->display(allocator);
    popBlackFromLink(proConBlockLink, allocator);
    allocator->display(allocator);
    destroyProConBlockLink(proConBlockLink, allocator);
    allocator->display(allocator);

}