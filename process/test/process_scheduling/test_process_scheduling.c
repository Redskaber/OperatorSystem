/*
 User: Redskaber
 Date: 2023/11/30
 Time: 15:27
*/
#include "test_process_scheduling.h"


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

static ProConBlock *test_create_ProConBlock() {
    ProConBlock *tp = initProConBlock(0x0001, "process-0x0001", 20, normal, callback);
    return tp;
}

static ProConBlockLink *test_create_ProConBlockLink() {
    ProConBlockLink *tpl = initProConBlockLink();
    return tpl;
}

static void test_pushToLink() {
    ProConBlock *tp = test_create_ProConBlock();
    ProConBlockLink *tpl = test_create_ProConBlockLink();
    pushToLink(tp, tpl);
}

void test_process_scheduling() {
    ProConBlock *proConBlock1 = initProConBlock(0x0001, "process-0x0001", 20, normal, callback);
    ProConBlock *proConBlock2 = initProConBlock(0x0002, "process-0x0002", 10, low, callback);
    ProConBlock *proConBlock3 = initProConBlock(0x0003, "process-0x0003", 30, normal, callback);
    ProConBlock *proConBlock4 = initProConBlock(0x0004, "process-0x0004", 50, high, callback);
    ProConBlock *proConBlock5 = initProConBlock(0x0005, "process-0x0005", 20, exigency, callback);
//    displayProConBlock(proConBlock);
//    destroyProConBlock(proConBlock);

    ProConBlockLink *proConBlockLink = initProConBlockLink();
    // push
//    pushToLink(proConBlock1, proConBlockLink);
//    pushToLink(proConBlock2, proConBlockLink);
//    pushToLink(proConBlock3, proConBlockLink);
//    pushToLink(proConBlock4, proConBlockLink);
//    pushToLink(proConBlock5, proConBlockLink);

    insertToLinkFromParam(proConBlockLink, proConBlock1, priorityCompare);
    insertToLinkFromParam(proConBlockLink, proConBlock2, priorityCompare);
    insertToLinkFromParam(proConBlockLink, proConBlock3, priorityCompare);
    insertToLinkFromParam(proConBlockLink, proConBlock4, priorityCompare);
    insertToLinkFromParam(proConBlockLink, proConBlock5, priorityCompare);
//    displayProConBlockLink(proConBlockLink);
//    destroyProConBlockLink(proConBlockLink);

//    popBlackFromLink(proConBlockLink);
//    popBlackFromLink(proConBlockLink);
//    popBlackFromLink(proConBlockLink);
//    popBlackFromLink(proConBlockLink);
//    popBlackFromLink(proConBlockLink);
//    popBlackFromLink(proConBlockLink);
//    popBlackFromLink(proConBlockLink);


//    popFrontFromLink(proConBlockLink);
//    popFrontFromLink(proConBlockLink);
//    popFrontFromLink(proConBlockLink);
//    popFrontFromLink(proConBlockLink);
//    popFrontFromLink(proConBlockLink);
//    popFrontFromLink(proConBlockLink);
//    popFrontFromLink(proConBlockLink);


    // priority sort
    // 1
    // 1 2
    // 3 1 2
    // 4 3 1 2
    // 5 4 3 1 2
//    priorityInsertLink(proConBlock1, proConBlockLink);
//    priorityInsertLink(proConBlock2, proConBlockLink);
//    priorityInsertLink(proConBlock3, proConBlockLink);
//    priorityInsertLink(proConBlock4, proConBlockLink);
//    priorityInsertLink(proConBlock5, proConBlockLink);

    // 3 2 1 0 1
    // 3 2 1 0 1
    // 3 2 1 1 0
    // 3 2 1 1 0
//    prioritySort(proConBlockLink);


    // check process order
//    displayProConBlockLink(proConBlockLink);
//    reverseProConBlockFromLink(proConBlockLink);
//    FirstComeFirstServe(proConBlockLink);
//    displayProConBlockLink(proConBlockLink);


    // running
//    runningProConBlockFromLink(proConBlockLink, NULL, roundRobinScheduling);
//    shortestJobNext(proConBlockLink);
//    sortLinkFromLinkParam(proConBlockLink, priorityCompare);
    displayProConBlockLink(proConBlockLink);

//    sortLinkFromLinkParam(proConBlockLink, totalTimeCompare);
//    displayProConBlockLink(proConBlockLink);

    destroyProConBlockLink(proConBlockLink);
//    FirstComeFirstServe(proConBlockLink);
//    displayProConBlockLink(proConBlockLink);

}