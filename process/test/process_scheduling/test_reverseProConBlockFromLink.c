/*
 User: Redskaber
 Date: 2023/12/17
 Time: 17:27
*/
#include "../header/test_reverseProConBlockFromLink.h"


void test_reverseProConBlockFromLink_whenLinkIsEmpty_doesNotCrash() {
    Allocator *allocator = createAllocator(ALLOCATE_TOTAL_SIZE);
    ProConBlockLink *proConBlockLink = initProConBlockLink(allocator);

    reverseProConBlockFromLink(proConBlockLink);

    destroyProConBlockLink(proConBlockLink, allocator);
}

void test_reverseProConBlockFromLink_whenLinkHasOneElement_doesNotChangeOrder() {
    Allocator *allocator = createAllocator(ALLOCATE_TOTAL_SIZE);
    ProConBlockLink *proConBlockLink = initProConBlockLink(allocator);
    ProConBlock *proConBlock = initProConBlock(1, "test", 1.0, normal, NULL, allocator);
    pushToLink(proConBlock, proConBlockLink);

    reverseProConBlockFromLink(proConBlockLink);

    assert(proConBlockLink->headProConBlock->aftProConBlock == proConBlock);
    assert(proConBlockLink->lastProConBlock == proConBlock);

    destroyProConBlockLink(proConBlockLink, allocator);
}

void test_reverseProConBlockFromLink_whenLinkHasMultipleElements_reversesOrder() {
    Allocator *allocator = createAllocator(ALLOCATE_TOTAL_SIZE);
    ProConBlockLink *proConBlockLink = initProConBlockLink(allocator);
    ProConBlock *proConBlock1 = initProConBlock(1, "test1", 1.0, normal, NULL, allocator);
    ProConBlock *proConBlock2 = initProConBlock(2, "test2", 2.0, normal, NULL, allocator);
    pushToLink(proConBlock1, proConBlockLink);
    pushToLink(proConBlock2, proConBlockLink);

    assert(proConBlockLink->headProConBlock->aftProConBlock == proConBlock2);
    assert(proConBlockLink->lastProConBlock == proConBlock1);

    reverseProConBlockFromLink(proConBlockLink);

    assert(proConBlockLink->headProConBlock->aftProConBlock == proConBlock1);
    assert(proConBlockLink->lastProConBlock == proConBlock2);

    destroyProConBlockLink(proConBlockLink, allocator);
}
