/*
 User: Redskaber
 Date: 2023/12/14
 Time: 14:24
*/
#include "memory_network.h"

static _Bool connect(NetWork *netWork, int member);

static _Bool disconnect(NetWork *netWork, int remember);

static void display(const NetWork *netWork);


/**
 * @brief Creates a new NetWork structure.
 *
 * This function allocates memory for a new NetWork structure and initializes its fields.
 * It sets the total, used, and remaining fields to the provided total value, 0, and total value respectively.
 * It also assigns the connect, disconnect, and display functions to the corresponding function pointers in the NetWork structure.
 *
 * @param total The total amount of NetWork resources.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created NetWork structure.
 */
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

/**
 * @brief Attempts to connect to the network.
 *
 * This function tries to allocate a specified amount of network resources.
 * It first checks if the requested amount of resources can be allocated without exceeding the total network resources.
 * If the allocation is possible, it increases the used resources by the requested amount, decreases the remaining resources by the same amount, and returns true.
 * If the allocation is not possible, it returns false.
 *
 * @param netWork Pointer to the NetWork structure representing the network resources.
 * @param member The amount of network resources requested.
 * @return Boolean value indicating whether the allocation was successful.
 */
static _Bool connect(NetWork *netWork, int member) {
    if (netWork->used + member <= netWork->total) {
        netWork->used += member;
        netWork->remain -= member;
        return true;
    }
    return false;
}

/**
 * @brief Attempts to disconnect from the network.
 *
 * This function tries to free up a specified amount of network resources.
 * It first checks if the requested amount of resources can be freed without exceeding the total network resources.
 * If the freeing up is possible, it decreases the used resources by the requested amount, increases the remaining resources by the same amount, and returns true.
 * If the freeing up is not possible, it returns false.
 *
 * @param netWork Pointer to the NetWork structure representing the network resources.
 * @param remember The amount of network resources to be freed.
 * @return Boolean value indicating whether the freeing up was successful.
 */
static _Bool disconnect(NetWork *netWork, int remember) {
    if (netWork->remain + remember <= netWork->total) {
        netWork->used -= remember;
        netWork->remain += remember;
        return true;
    }
    return false;
}

/**
 * @brief Displays the details of a NetWork structure.
 *
 * This function prints the details of the NetWork structure to the console.
 * It displays the total, used, and remaining network resources.
 *
 * @param netWork Pointer to the NetWork structure to be displayed.
 */
static void display(const NetWork *netWork) {
    printf_s("########################################\n");
    printf_s("NetWork:\n");
    printf_s("\ttotal: %d\n", netWork->total);
    printf_s("\tused: %d\n", netWork->used);
    printf_s("\tremain: %d\n", netWork->remain);
    printf_s("########################################\n");
}

/**
 * @brief Destroys a NetWork structure.
 *
 * This function deallocates the memory used by the NetWork structure.
 * It first checks if the NetWork pointer is not NULL.
 * If it is not NULL, it uses the provided Allocator to deallocate the memory used by the NetWork structure and sets the NetWork pointer to NULL.
 *
 * @param netWork Pointer to the NetWork structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyNetWork(NetWork *netWork, Allocator *allocator) {
    if (netWork != NULL) {
        allocator->deallocate(allocator, netWork, sizeof(NetWork));
        netWork = NULL;
    }
}
