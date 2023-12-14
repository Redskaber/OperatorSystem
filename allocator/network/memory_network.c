/*
 User: Redskaber
 Date: 2023/12/14
 Time: 14:24
*/
#include "memory_network.h"

static _Bool connect(NetWork *netWork, int member);

static _Bool disconnect(NetWork *netWork, int remember);

static void display(const NetWork *netWork);

NetWork *createNetWork(int total, Allocator *allocator) {

    NetWork *newNetWork = NULL;
    newNetWork = allocator->allocate(allocator, sizeof(NetWork));
    assert(newNetWork != NULL);

    newNetWork->total = total;
    newNetWork->used = 0;
    newNetWork->remain = total;

    newNetWork->connect = connect;
    newNetWork->disconnect = disconnect;
    newNetWork->display = display;
    return newNetWork;
}


static _Bool connect(NetWork *netWork, int member) {
    if (netWork->used + member <= netWork->total) {
        netWork->used += member;
        netWork->remain -= member;
        return true;
    }
    return false;
}

static _Bool disconnect(NetWork *netWork, int remember) {
    if (netWork->remain + remember <= netWork->total) {
        netWork->used -= remember;
        netWork->remain += remember;
        return true;
    }
    return false;
}

static void display(const NetWork *netWork) {
    printf_s("########################################\n");
    printf_s("NetWork:\n");
    printf_s("\ttotal: %d\n", netWork->total);
    printf_s("\tused: %d\n", netWork->used);
    printf_s("\tremain: %d\n", netWork->remain);
    printf_s("########################################\n");
}

void destroyNetWork(NetWork *netWork, Allocator *allocator) {
    if (netWork != NULL) {
        allocator->deallocate(allocator, netWork, sizeof(NetWork));
        netWork = NULL;
    }
}