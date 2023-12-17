/*
 User: Redskaber
 Date: 2023/11/28
 Time: 10:51
*/
#include "process_scheduling.h"



//创建进程： 系统接收到新的任务或作业时，创建相应的进程
//创建进程控制块
//就绪队列： 进程进入就绪队列，等待系统调度。
//调度算法选择： 调度器根据调度算法从就绪队列中选择一个进程。
//执行进程： 选中的进程在 CPU 上执行一段时间片。
//阻塞检查： 检查进程是否因为等待某个事件而阻塞。
//如果是，将进程状态改为阻塞状态，进程移出 CPU。
//如果否，进程继续执行。
//唤醒： 当等待的事件发生时，被阻塞的进程被唤醒，重新进入就绪队列。
//终止检查： 检查进程是否执行完成或由于异常情况而终止。
//如果是，将进程状态改为终止状态，释放资源。
//如果否，进程继续执行。
//循环执行： 以上步骤循环执行，系统选择下一个要执行的进程。



/**
 * @brief Creates a new ProConBlock structure and initializes it as the head of a linked list.
 *
 * This function allocates memory for a new ProConBlock structure and initializes its fields.
 * It sets the process ID to 0, the process name to "HEAD", and the process state to new.
 * It also sets the process priority, total time, and execute time to 0, and the callback function to NULL.
 * The previous and next ProConBlock pointers are set to NULL, indicating that this ProConBlock is the head of a linked list.
 *
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created ProConBlock structure.
 */
static ProConBlock *headProConBlock(Allocator *allocator) {

    ProConBlock *head = allocator->allocate(allocator, sizeof(ProConBlock));
    head->p_id = 0x0;
    head->p_name = "HEAD";
    head->p_state = new;

    head->p_priority = 0;
    head->p_total_time = 0;
    head->p_execute_time = 0;
    head->callback = NULL;

    head->perProConBlock = NULL;
    head->aftProConBlock = NULL;
    return head;
}


/**
 * @brief Initializes a ProConBlock structure.
 *
 * This function allocates memory for a new ProConBlock structure and initializes its fields.
 * It first creates a head ProConBlock by calling the headProConBlock function.
 * It then sets the process ID, name, state, priority, total time, execute time, and callback function to the provided values.
 * The previous and next ProConBlock pointers are set to NULL, indicating that this ProConBlock is not linked to any other ProConBlocks.
 *
 * @param p_id The process ID to be assigned to the ProConBlock.
 * @param p_name The process name to be assigned to the ProConBlock.
 * @param p_total_time The total time to be assigned to the ProConBlock.
 * @param p_priority The process priority to be assigned to the ProConBlock.
 * @param callBack The callback function to be assigned to the ProConBlock.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created ProConBlock structure.
 */
ProConBlock *
initProConBlock(
        int p_id,
        char *p_name,
        double p_total_time,
        ProcessPriority p_priority,
        CallBack callBack,
        Allocator *allocator
) {

    ProConBlock *newProConBlock = headProConBlock(allocator);

    newProConBlock->p_id = p_id;
    newProConBlock->p_name = p_name;
    newProConBlock->p_state = new;

    newProConBlock->p_priority = p_priority;
    newProConBlock->p_total_time = p_total_time;
    newProConBlock->p_execute_time = 0;

    newProConBlock->callback = callBack;

    newProConBlock->perProConBlock = NULL;
    newProConBlock->aftProConBlock = NULL;

    return newProConBlock;
}

/**
 * @brief Destroys a ProConBlock structure.
 *
 * This function deallocates the memory used by the ProConBlock structure.
 * It first checks if the ProConBlock pointer is not NULL.
 * If it is not NULL, it checks if the aftProConBlock pointer in the ProConBlock is not NULL.
 * If the aftProConBlock pointer is not NULL, it recursively destroys the aftProConBlock.
 * It then deallocates the memory used by the ProConBlock structure and sets the ProConBlock pointer to NULL.
 *
 * @param proConBlock Pointer to the ProConBlock structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyProConBlock(ProConBlock *proConBlock, Allocator *allocator) {

    if (proConBlock != NULL) {
        if (proConBlock->aftProConBlock != NULL) {
            destroyProConBlock(proConBlock->aftProConBlock, allocator);
        }
        allocator->deallocate(allocator, proConBlock, sizeof(ProConBlock));
        proConBlock = NULL;

    }
}

/**
 * @brief Displays the details of a ProConBlock structure.
 *
 * This function prints the details of the ProConBlock structure to the console.
 * It displays the process ID, process name, process state, process priority, total time, and execute time.
 * The process state and process priority are displayed as strings, converted from their respective enum values.
 *
 * @param proConBlock Pointer to the ProConBlock structure to be displayed.
 */
void displayProConBlock(ProConBlock *proConBlock) {

    printf_s("###################################\n");
    printf_s("process:\n");
    printf_s("\tprocess_id: %d\n", proConBlock->p_id);
    printf_s("\tprocess_name: %s\n", proConBlock->p_name);
    printf_s("\tprocess_state: %s\n", proStateToString(proConBlock->p_state));
    printf_s("\tprocess_priority: %s\n", proPriorityToString(proConBlock->p_priority));
    printf_s("\tprocess_total_time: %f\n", proConBlock->p_total_time);
    printf_s("\tprocess_execute_time: %f\n", proConBlock->p_execute_time);
    printf_s("###################################\n");
}


/**
 * @brief Initializes a ProConBlockLink structure.
 *
 * This function allocates memory for a new ProConBlockLink structure and initializes its fields.
 * It creates a head ProConBlock by calling the headProConBlock function and assigns it to the headProConBlock field of the ProConBlockLink.
 * It also sets the lastProConBlock field of the ProConBlockLink to the headProConBlock, indicating that the ProConBlockLink currently contains only the head ProConBlock.
 *
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created ProConBlockLink structure.
 */
ProConBlockLink *initProConBlockLink(Allocator *allocator) {

    ProConBlockLink *newProConBlockLink = NULL;
    newProConBlockLink = allocator->allocate(allocator, sizeof(ProConBlockLink));
    newProConBlockLink->headProConBlock = headProConBlock(allocator);
    newProConBlockLink->lastProConBlock = newProConBlockLink->headProConBlock;

    return newProConBlockLink;
}

/**
 * @brief Destroys a ProConBlockLink structure.
 *
 * This function deallocates the memory used by the ProConBlockLink structure.
 * It first checks if the ProConBlockLink pointer is not NULL.
 * If it is not NULL, it destroys the head ProConBlock in the ProConBlockLink by calling the destroyProConBlock function.
 * It then deallocates the memory used by the ProConBlockLink structure itself.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyProConBlockLink(ProConBlockLink *proConBlockLink, Allocator *allocator) {

    if (proConBlockLink != NULL) {
        destroyProConBlock(proConBlockLink->headProConBlock, allocator);
        allocator->deallocate(allocator, proConBlockLink, sizeof(ProConBlockLink));
    }
}

/**
 * @brief Adds a ProConBlock to a ProConBlockLink.
 *
 * This function adds a ProConBlock to a ProConBlockLink.
 * If the aftProConBlock field of the headProConBlock in the ProConBlockLink is NULL,
 * it means the ProConBlockLink is empty. In this case, it adds the ProConBlock to the
 * aftProConBlock field of the headProConBlock and sets the lastProConBlock field of the
 * ProConBlockLink to the ProConBlock.
 * If the aftProConBlock field of the headProConBlock in the ProConBlockLink is not NULL,
 * it means the ProConBlockLink already contains ProConBlocks. In this case, it inserts
 * the ProConBlock at the beginning of the ProConBlockLink, right after the headProConBlock.
 *
 * @param proConBlock Pointer to the ProConBlock to be added to the ProConBlockLink.
 * @param proConBlockLink Pointer to the ProConBlockLink where the ProConBlock will be added.
 */
void pushToLink(ProConBlock *proConBlock, ProConBlockLink *proConBlockLink) {

    if (proConBlockLink->headProConBlock->aftProConBlock == NULL) {
        // [] -> []
        proConBlockLink->headProConBlock->aftProConBlock = proConBlock;
        proConBlockLink->lastProConBlock = proConBlock;
    } else {
        // <-[z]->
        // [h] -> [1] <->[] <-> []
        // [h] -> [z] <->[1] <-> [] <-> []
        proConBlock->perProConBlock = NULL;
        proConBlock->aftProConBlock = proConBlockLink->headProConBlock->aftProConBlock;

        proConBlockLink->headProConBlock->aftProConBlock->perProConBlock = proConBlock;
        proConBlockLink->headProConBlock->aftProConBlock = proConBlock;
    }
}

/**
 * @brief Inserts a ProConBlock into a ProConBlockLink at a specific position.
 *
 * This function inserts a ProConBlock into a ProConBlockLink right after the specified orderProConBlock.
 * It first detaches the insertProConBlock from its current position in the ProConBlockLink.
 * If the insertProConBlock is the last ProConBlock in the ProConBlockLink, it updates the lastProConBlock field of the ProConBlockLink.
 * It then inserts the insertProConBlock right after the orderProConBlock.
 * If the orderProConBlock is the headProConBlock of the ProConBlockLink, it sets the perProConBlock field of the insertProConBlock to NULL.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink where the ProConBlock will be inserted.
 * @param orderProConBlock Pointer to the ProConBlock after which the ProConBlock will be inserted.
 * @param insertProConBlock Pointer to the ProConBlock to be inserted into the ProConBlockLink.
 */
inline static void
insertProConBlockToOrderLink(
        ProConBlockLink *proConBlockLink,
        ProConBlock *orderProConBlock,
        ProConBlock *insertProConBlock
) {
    // Detach the insertProConBlock from its current position in the ProConBlockLink
    insertProConBlock->perProConBlock->aftProConBlock = insertProConBlock->aftProConBlock;
    if (insertProConBlock->aftProConBlock != NULL) {
        insertProConBlock->aftProConBlock->perProConBlock = insertProConBlock->perProConBlock;
    } else {
        // If the insertProConBlock is the last ProConBlock in the ProConBlockLink, update the lastProConBlock field of the ProConBlockLink
        proConBlockLink->lastProConBlock = insertProConBlock->perProConBlock;
    }

    // Insert the insertProConBlock right after the orderProConBlock
    if (orderProConBlock != proConBlockLink->headProConBlock) {
        insertProConBlock->perProConBlock = orderProConBlock;
    } else {
        insertProConBlock->perProConBlock = NULL;
    }

    insertProConBlock->aftProConBlock = orderProConBlock->aftProConBlock;
    orderProConBlock->aftProConBlock->perProConBlock = insertProConBlock;
    orderProConBlock->aftProConBlock = insertProConBlock;
}

/**
 * @brief Sorts a ProConBlockLink based on a provided comparison function.
 *
 * This function sorts a ProConBlockLink using an algorithm similar to insertion sort.
 * It iterates over the ProConBlockLink, and for each ProConBlock, it finds the correct position for it based on the provided comparison function.
 * If the ProConBlock needs to be moved, it is inserted at the correct position by calling the insertProConBlockToOrderLink function.
 * If the ProConBlock does not need to be moved, the function moves on to the next ProConBlock.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink to be sorted.
 * @param compare Function pointer to the comparison function used for sorting. The comparison function should take two ProConBlock pointers as parameters and return a boolean value indicating whether the first ProConBlock should come before the second ProConBlock in the sorted ProConBlockLink.
 */
void sortLinkFromLinkParam(ProConBlockLink *proConBlockLink, Compare compare) {

    // If the ProConBlockLink is empty or contains only one ProConBlock, there is nothing to sort
    if (proConBlockLink->headProConBlock->aftProConBlock == NULL ||
        proConBlockLink->headProConBlock->aftProConBlock == proConBlockLink->lastProConBlock)
        return;

    ProConBlock *order = proConBlockLink->headProConBlock->aftProConBlock;
    ProConBlock *insertData = NULL;
    ProConBlock *insertIndex = NULL;

    while (order->aftProConBlock != NULL) {
        insertData = order->aftProConBlock;
        insertIndex = order;

        // Find the correct position for the insertData ProConBlock
        while (insertIndex != NULL && compare(insertData, insertIndex)) {
            insertIndex = insertIndex->perProConBlock;
        }
        if (insertIndex == NULL) {
            insertIndex = proConBlockLink->headProConBlock;
        }

        // If the insertData ProConBlock needs to be moved, insert it at the correct position
        if (insertIndex != order) {
            insertProConBlockToOrderLink(proConBlockLink, insertIndex, insertData);
        } else {
            // If the insertData ProConBlock does not need to be moved, move on to the next ProConBlock
            order = order->aftProConBlock;
        }

    }
}

/**
 * @brief Removes the last ProConBlock from a ProConBlockLink.
 *
 * This function removes the last ProConBlock from a ProConBlockLink and deallocates the memory used by it.
 * If the last ProConBlock is not NULL, it checks if it is the only ProConBlock in the ProConBlockLink (i.e., it is the next ProConBlock of the headProConBlock).
 * If it is not the only ProConBlock, it retrieves the previous ProConBlock of the last ProConBlock, destroys the last ProConBlock, and updates the lastProConBlock field of the ProConBlockLink to the previous ProConBlock.
 * If it is the only ProConBlock, it destroys the last ProConBlock and sets the aftProConBlock field of the headProConBlock and the lastProConBlock field of the ProConBlockLink to NULL.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink from which the last ProConBlock will be removed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void popBlackFromLink(ProConBlockLink *proConBlockLink, Allocator *allocator) {

    if (proConBlockLink->lastProConBlock != NULL) {
        if (proConBlockLink->headProConBlock->aftProConBlock != proConBlockLink->lastProConBlock) {
            // [h] -> [] <-> [] <-> []
            ProConBlock *popProConBlock = proConBlockLink->lastProConBlock;
            ProConBlock *perProConBlock = popProConBlock->perProConBlock;
            destroyProConBlock(popProConBlock, allocator);

            perProConBlock->aftProConBlock = NULL;
            proConBlockLink->lastProConBlock = perProConBlock;
        } else {
            // [h] -> []
            destroyProConBlock(proConBlockLink->headProConBlock->aftProConBlock, allocator);
            proConBlockLink->headProConBlock->aftProConBlock = NULL;
            proConBlockLink->lastProConBlock = NULL;
        }
    }
}

/**
 * @brief Removes the first ProConBlock from a ProConBlockLink.
 *
 * This function removes the first ProConBlock (the one after the head) from a ProConBlockLink and deallocates the memory used by it.
 * If the aftProConBlock field of the headProConBlock in the ProConBlockLink is not NULL, it means the ProConBlockLink contains ProConBlocks.
 * In this case, it retrieves the next ProConBlock of the first ProConBlock, destroys the first ProConBlock, and sets the aftProConBlock field of the headProConBlock to the next ProConBlock.
 * If the next ProConBlock is not NULL, it sets the perProConBlock field of the next ProConBlock to NULL.
 * If the next ProConBlock is NULL, it means the ProConBlockLink is now empty, so it sets the lastProConBlock field of the ProConBlockLink to NULL.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink from which the first ProConBlock will be removed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void popFrontFromLink(ProConBlockLink *proConBlockLink, Allocator *allocator) {

    if (proConBlockLink->headProConBlock->aftProConBlock != NULL) {
        // [h] -> [] <-> [] <-> []
        ProConBlock *aftProConBlock = proConBlockLink->headProConBlock->aftProConBlock->aftProConBlock;
        proConBlockLink->headProConBlock->aftProConBlock->aftProConBlock = NULL;
        destroyProConBlock(proConBlockLink->headProConBlock->aftProConBlock, allocator);
        proConBlockLink->headProConBlock->aftProConBlock = aftProConBlock;
        if (aftProConBlock != NULL) {
            aftProConBlock->perProConBlock = NULL;
        } else {
            proConBlockLink->lastProConBlock = NULL;
        }
    }
}

/**
 * @brief Inserts a ProConBlock into a ProConBlockLink based on a provided comparison function.
 *
 * This function inserts a ProConBlock into a ProConBlockLink in a position determined by the provided comparison function.
 * If the ProConBlockLink is empty, it adds the ProConBlock as the first element after the head.
 * If the ProConBlockLink is not empty, it finds the correct position for the ProConBlock by iterating over the ProConBlockLink and using the comparison function.
 * If the ProConBlock should be inserted at the beginning of the ProConBlockLink, it calls the pushToLink function to insert it.
 * If the ProConBlock should be inserted in the middle or at the end of the ProConBlockLink, it adjusts the perProConBlock and aftProConBlock pointers of the surrounding ProConBlocks to insert it.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink where the ProConBlock will be inserted.
 * @param proConBlock Pointer to the ProConBlock to be inserted into the ProConBlockLink.
 * @param compare Function pointer to the comparison function used for determining the position of the ProConBlock in the ProConBlockLink. The comparison function should take two ProConBlock pointers as parameters and return a boolean value indicating whether the first ProConBlock should come before the second ProConBlock in the ProConBlockLink.
 */
void insertToLinkFromParam(ProConBlockLink *proConBlockLink, ProConBlock *proConBlock, Compare compare) {

    if (proConBlockLink->headProConBlock->aftProConBlock == NULL) {
        // [] -> []
        proConBlockLink->headProConBlock->aftProConBlock = proConBlock;
        proConBlockLink->lastProConBlock = proConBlock;
    } else {
        // <-[2]->
        // [h] -> [3] <->[2] <-> [1]
        // [h] -> [3] <->[2] <-> [2] <-> [1]
        if (compare(proConBlock, proConBlockLink->headProConBlock->aftProConBlock)) {
            pushToLink(proConBlock, proConBlockLink);
        } else {
            ProConBlock *temp = proConBlockLink->headProConBlock->aftProConBlock->aftProConBlock;
            while (temp != NULL && compare(temp, proConBlock)) {
                temp = temp->aftProConBlock;
            }
            if (temp != NULL) {
                proConBlock->perProConBlock = temp->perProConBlock;
                proConBlock->aftProConBlock = temp;
                temp->perProConBlock->aftProConBlock = proConBlock;
                temp->perProConBlock = proConBlock;
            } else {
                proConBlockLink->lastProConBlock->aftProConBlock = proConBlock;
                proConBlock->perProConBlock = proConBlockLink->lastProConBlock;
                proConBlockLink->lastProConBlock = proConBlock;
                proConBlock->aftProConBlock = NULL;
            }
        }
    }
}

/**
 * @brief Reverses the order of ProConBlocks in a ProConBlockLink.
 *
 * This function reverses the order of ProConBlocks in a ProConBlockLink.
 * It starts from the head of the ProConBlockLink and iteratively moves each ProConBlock to the end of the ProConBlockLink.
 * The function continues until all ProConBlocks have been moved, resulting in the original order being reversed.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink whose ProConBlocks will be reversed.
 */
void reverseProConBlockFromLink(ProConBlockLink *proConBlockLink) {

    ProConBlock *orderProConBlock = proConBlockLink->headProConBlock->aftProConBlock;
    ProConBlock *insertProConBlock = proConBlockLink->lastProConBlock;
    ProConBlock *oldFirstProConBlock = proConBlockLink->headProConBlock->aftProConBlock;

    // If the ProConBlockLink is empty or contains only one ProConBlock, there is nothing to reverse
    if (orderProConBlock == NULL || insertProConBlock == NULL || orderProConBlock == insertProConBlock)
        return;

    while (true) {
        // oldFirstProConBlock    ↓
        // orderProConBlock       ↓                    ↓    insertProConBlock
        // link           [H] -> [] <-> [] <-> [] <-> [] -> NULL
        ProConBlock *newLastProConBlock = proConBlockLink->lastProConBlock->perProConBlock;
        newLastProConBlock->aftProConBlock = NULL;

        if (orderProConBlock == oldFirstProConBlock) {

            insertProConBlock->perProConBlock = NULL;
            insertProConBlock->aftProConBlock = orderProConBlock;

            orderProConBlock->perProConBlock = insertProConBlock;
            proConBlockLink->headProConBlock->aftProConBlock = insertProConBlock;

            orderProConBlock = insertProConBlock;
            insertProConBlock = newLastProConBlock;
            proConBlockLink->lastProConBlock = newLastProConBlock;

            if (newLastProConBlock == oldFirstProConBlock) {
                break;
            }

        } else {
            if (orderProConBlock->aftProConBlock != NULL) {
                insertProConBlock->aftProConBlock = orderProConBlock->aftProConBlock;
                orderProConBlock->aftProConBlock->perProConBlock = insertProConBlock;
            }
            insertProConBlock->perProConBlock = orderProConBlock;
            orderProConBlock->aftProConBlock = insertProConBlock;

            orderProConBlock = insertProConBlock;
            insertProConBlock = newLastProConBlock;
            if (orderProConBlock->aftProConBlock == NULL) {
                break;
            }

            proConBlockLink->lastProConBlock = newLastProConBlock;
        }
    }
}

/**
 * @brief Implements the First-Come-First-Serve scheduling algorithm for a ProConBlockLink.
 *
 * This function implements the First-Come-First-Serve scheduling algorithm for a ProConBlockLink.
 * It simply reverses the order of ProConBlocks in the ProConBlockLink by calling the reverseProConBlockFromLink function.
 * As a result, the ProConBlock that was added first will be the first one to be executed, and the ProConBlock that was added last will be the last one to be executed.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink to be scheduled.
 */
void firstComeFirstServe(ProConBlockLink *proConBlockLink) {
    reverseProConBlockFromLink(proConBlockLink);
}

/**
 * @brief Inserts a ProConBlock into a ProConBlockLink after a specific ProConBlock with the same priority.
 *
 * This function inserts a ProConBlock (quick) into a ProConBlockLink right after a specified ProConBlock (slow).
 * It first detaches the quick ProConBlock from its current position in the ProConBlockLink.
 * If the quick ProConBlock is not the last ProConBlock in the ProConBlockLink, it updates the perProConBlock and aftProConBlock pointers of the surrounding ProConBlocks.
 * It then inserts the quick ProConBlock right after the slow ProConBlock.
 * If the slow ProConBlock is the first ProConBlock in the ProConBlockLink (i.e., it is the next ProConBlock of the headProConBlock), it sets the perProConBlock field of the quick ProConBlock to NULL.
 * Otherwise, it adjusts the perProConBlock and aftProConBlock pointers of the surrounding ProConBlocks to insert the quick ProConBlock.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink where the ProConBlock will be inserted.
 * @param slow Pointer to the ProConBlock after which the ProConBlock will be inserted.
 * @param quick Pointer to the ProConBlock to be inserted into the ProConBlockLink.
 */
inline static void
insertProConBlockToSamePriorityAfter(
        ProConBlockLink *proConBlockLink,
        ProConBlock *slow,
        ProConBlock *quick
) {
    // slow   ↓
    // quick  ↓
    // [h] -> [] <-> [] <-> [] <-> [] -> NULL

    // 提取quick
    quick->perProConBlock->aftProConBlock = quick->aftProConBlock;
    if (quick->aftProConBlock != NULL) {
        quick->aftProConBlock->perProConBlock = quick->perProConBlock;
    }

    // insert quick
    if (slow == proConBlockLink->headProConBlock->aftProConBlock) {
        quick->aftProConBlock = slow;
        slow->perProConBlock = quick;
        quick->perProConBlock = NULL;
        proConBlockLink->headProConBlock->aftProConBlock = quick;
    } else {
        quick->aftProConBlock = slow;
        quick->perProConBlock = slow->perProConBlock;
        slow->perProConBlock->aftProConBlock = quick;
        slow->perProConBlock = quick;
    }
}

/**
 * @brief Orders ProConBlocks in a ProConBlockLink based on a specified priority.
 *
 * This function iterates over a ProConBlockLink and for each ProConBlock with the specified priority, it finds the correct position for it.
 * The correct position is determined by the order of the ProConBlocks in the ProConBlockLink and the specified priority.
 * If a ProConBlock with the specified priority needs to be moved, it is inserted right after the specified slow ProConBlock by calling the insertProConBlockToSamePriorityAfter function.
 * The function continues until all ProConBlocks have been checked, and returns the last ProConBlock with the specified priority.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink to be ordered.
 * @param slow Pointer to the ProConBlock after which ProConBlocks with the specified priority will be inserted.
 * @param quick Pointer to the ProConBlock from where the function starts checking for ProConBlocks with the specified priority.
 * @param priority The priority based on which the ProConBlocks will be ordered.
 * @return Pointer to the last ProConBlock with the specified priority.
 */
inline static ProConBlock *
orderProConBlockThroughPriority(
        ProConBlockLink *proConBlockLink,
        ProConBlock *slow,
        ProConBlock *quick,
        ProcessPriority priority
) {

    while (quick != NULL) {
        if (quick->p_priority == priority) {
            if (slow != quick) {
                insertProConBlockToSamePriorityAfter(proConBlockLink, slow, quick);
            }
            slow = slow->aftProConBlock;
        }
        quick = quick->aftProConBlock;
    }
    return slow;
}

/**
 * @brief Schedules ProConBlocks in a ProConBlockLink based on their priority.
 *
 * This function orders the ProConBlocks in a ProConBlockLink based on their priority.
 * It first checks if the ProConBlockLink is empty or contains only one ProConBlock. If so, it returns without making any changes.
 * If the ProConBlockLink contains more than one ProConBlock, it starts from the first ProConBlock (the one after the head) and orders the ProConBlocks based on their priority.
 * The ordering is done by calling the orderProConBlockThroughPriority function for each priority level, starting from exigency and ending with low.
 * The orderProConBlockThroughPriority function inserts each ProConBlock with the specified priority into the correct position in the ProConBlockLink.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink to be scheduled.
 */
void priorityScheduling(ProConBlockLink *proConBlockLink) {

    if (proConBlockLink->headProConBlock->aftProConBlock == NULL ||
        proConBlockLink->headProConBlock->aftProConBlock == proConBlockLink->lastProConBlock)
        return;

    // slow   ↓
    // quick  ↓
    // [h] -> [] <-> [] <-> [] <-> [] -> NULL
    ProConBlock *slow = proConBlockLink->headProConBlock->aftProConBlock;
    ProConBlock *quick = proConBlockLink->headProConBlock->aftProConBlock;

    slow = orderProConBlockThroughPriority(proConBlockLink, slow, quick, exigency);
    slow = orderProConBlockThroughPriority(proConBlockLink, slow, slow, high);
    slow = orderProConBlockThroughPriority(proConBlockLink, slow, slow, normal);
    orderProConBlockThroughPriority(proConBlockLink, slow, slow, low);
}

/**
 * @brief Executes all ProConBlocks in a ProConBlockLink.
 *
 * This function iterates over a ProConBlockLink and executes each ProConBlock.
 * For each ProConBlock, it first prints a message indicating the start of execution and displays the details of the ProConBlock.
 * It then sets the process state of the ProConBlock to running and calls the callback function of the ProConBlock.
 * After the callback function returns, it sets the execute time of the ProConBlock to the total time and the process state to suspended_ready.
 * It then displays the details of the ProConBlock again and prints a message indicating the end of execution.
 * The function continues until all ProConBlocks have been executed.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink whose ProConBlocks will be executed.
 */
void executeOver(ProConBlockLink *proConBlockLink) {

    ProConBlock *proConBlock = proConBlockLink->headProConBlock->aftProConBlock;
    while (proConBlock != NULL) {
        printf_s("Start running...\n");
        displayProConBlock(proConBlock);

        proConBlock->p_state = running;
        proConBlock = proConBlock->callback(proConBlock);
        proConBlock->p_execute_time = proConBlock->p_total_time;
        proConBlock->p_state = suspended_ready;

        displayProConBlock(proConBlock);
        printf_s("End running...\n");
        proConBlock = proConBlock->aftProConBlock;
    }
}

/**
 * @brief Removes a ProConBlock from a ProConBlockLink.
 *
 * This function removes a ProConBlock (referred to as loopLink) from a ProConBlockLink.
 * It first updates the aftProConBlock pointer of the previous ProConBlock and the perProConBlock pointer of the next ProConBlock to bypass the loopLink ProConBlock.
 * It then returns the loopLink ProConBlock, which has been removed from the ProConBlockLink but is still allocated.
 *
 * @param loopLink Pointer to the ProConBlock to be removed from the ProConBlockLink.
 * @return Pointer to the removed ProConBlock.
 */
inline static ProConBlock *removeNextProConBlockFromLink(ProConBlock *loopLink) {

    loopLink->perProConBlock->aftProConBlock = loopLink->aftProConBlock;
    loopLink->aftProConBlock->perProConBlock = loopLink->perProConBlock;
    return loopLink;
}

/**
 * @brief Executes a ProConBlock and updates its state and execution time.
 *
 * This function executes a ProConBlock and updates its state and execution time based on the result of the execution.
 * It first sets the process state of the ProConBlock to running and displays its details.
 * It then checks if the execution time of the ProConBlock plus a time slice is greater than or equal to the total time of the ProConBlock, or if the ProConBlock is the only one in the ProConBlockLink.
 * If either condition is true, it calls the callback function of the ProConBlock, sets the execute time of the ProConBlock to the total time, and sets the process state to suspended_ready.
 * If neither condition is true, it calls the callback function of the ProConBlock, increments the execute time by a time slice, and sets the process state to suspended_blocked.
 * After the execution, it displays the details of the ProConBlock again and returns the ProConBlock.
 *
 * @param loopLink Pointer to the ProConBlock to be executed.
 * @return Pointer to the executed ProConBlock.
 */
ProConBlock *runningProConBlockTask(ProConBlock *loopLink) {

    printf_s("Start running...\n");
    loopLink->p_state = running;
    displayProConBlock(loopLink);

    if (loopLink->p_execute_time + TIME_SLICE >= loopLink->p_total_time ||
        loopLink->aftProConBlock == loopLink && loopLink->perProConBlock == loopLink
            ) {
        // read per state, execute func
        loopLink = loopLink->callback(loopLink);
        loopLink->p_execute_time = loopLink->p_total_time;
        loopLink->p_state = suspended_ready;

        displayProConBlock(loopLink);
        printf_s("End running...\n");

    } else {
        loopLink = loopLink->callback(loopLink);
        loopLink->p_execute_time += TIME_SLICE;
        loopLink->p_state = suspended_blocked;

        displayProConBlock(loopLink);
        printf_s("Stop running...\n");
    }

    return loopLink;
}

/**
 * @brief Reconfigures a ProConBlockLink by inserting a ProConBlock at the beginning.
 *
 * This function inserts a ProConBlock (referred to as cache) into a ProConBlockLink at the beginning, right after a specified ProConBlock (referred to as finishLink).
 * If finishLink is NULL, it means the ProConBlockLink is empty. In this case, it sets the perProConBlock and aftProConBlock fields of the cache ProConBlock to NULL, assigns cache to finishLink, and updates the lastProConBlock field of the ProConBlockLink to cache.
 * If finishLink is not NULL, it means the ProConBlockLink already contains ProConBlocks. In this case, it sets the perProConBlock field of the cache ProConBlock to NULL, the aftProConBlock field of the cache ProConBlock to finishLink, and the perProConBlock field of the finishLink ProConBlock to cache. It then assigns cache to finishLink.
 * The function returns the finishLink ProConBlock, which is the first ProConBlock in the ProConBlockLink after the head.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink where the ProConBlock will be inserted.
 * @param finishLink Pointer to the ProConBlock after which the ProConBlock will be inserted.
 * @param cache Pointer to the ProConBlock to be inserted into the ProConBlockLink.
 * @return Pointer to the first ProConBlock in the ProConBlockLink after the head.
 */
inline static ProConBlock *
reconfigurationProConBlockLink(
        ProConBlockLink *proConBlockLink,
        ProConBlock *finishLink,
        ProConBlock *cache
) {

    if (finishLink == NULL) {
        cache->perProConBlock = NULL;
        cache->aftProConBlock = NULL;
        finishLink = cache;
        // renew proConBlockLink lastProConBlock
        proConBlockLink->lastProConBlock = cache;

    } else {
        finishLink->perProConBlock = cache;
        cache->aftProConBlock = finishLink;
        cache->perProConBlock = NULL;
        finishLink = cache;
    }
    return finishLink;
}

/**
 * @brief Implements the Round Robin scheduling algorithm for a ProConBlockLink.
 *
 * This function implements the Round Robin scheduling algorithm for a ProConBlockLink.
 * It first sets up a loop link by connecting the last ProConBlock in the ProConBlockLink to the first ProConBlock.
 * It then enters a loop where it executes each ProConBlock in the ProConBlockLink for a time slice.
 * If a ProConBlock finishes execution (i.e., its execution time plus a time slice is greater than or equal to its total time) or if it is the only ProConBlock in the ProConBlockLink, it is moved to the beginning of the ProConBlockLink.
 * If a ProConBlock does not finish execution, it remains in its current position in the ProConBlockLink.
 * The function continues until all ProConBlocks have been executed and moved to the beginning of the ProConBlockLink.
 * Finally, it updates the headProConBlock field of the ProConBlockLink to point to the first ProConBlock in the ProConBlockLink and displays the details of the ProConBlockLink.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink to be scheduled.
 */
void roundRobinScheduling(ProConBlockLink *proConBlockLink) {

    ProConBlock *finishLink = NULL;
    ProConBlock *loopLink = proConBlockLink->headProConBlock->aftProConBlock;
    // loop link
    loopLink->perProConBlock = proConBlockLink->lastProConBlock;
    proConBlockLink->lastProConBlock->aftProConBlock = loopLink;

    while (true) {
        // last node handle
        if (loopLink->aftProConBlock == loopLink && loopLink->perProConBlock == loopLink) {
            loopLink = runningProConBlockTask(loopLink);
            finishLink = reconfigurationProConBlockLink(proConBlockLink, finishLink, loopLink);
            break;
        }
        if (loopLink->p_execute_time + TIME_SLICE >= loopLink->p_total_time) {
            loopLink = runningProConBlockTask(loopLink);
            ProConBlock *cache = loopLink;
            // remove
            // <-> H <-> [1] <-> [2] <-> [3] -> [1] <->
            loopLink = removeNextProConBlockFromLink(loopLink)->aftProConBlock;
            finishLink = reconfigurationProConBlockLink(proConBlockLink, finishLink, cache);
        } else {
            loopLink = runningProConBlockTask(loopLink)->aftProConBlock;
        }
    }

    // renew proConBlockLink headProConBlock
    proConBlockLink->headProConBlock->aftProConBlock = finishLink;
    displayProConBlockLink(proConBlockLink);
}

/**
 * @brief Compares the total time of two ProConBlocks.
 *
 * This function compares the total time of two ProConBlocks.
 * It casts the void pointers p1 and p2 to ProConBlock pointers and retrieves their total time.
 * It then checks if the total time of the ProConBlock pointed to by p1 is less than the total time of the ProConBlock pointed to by p2.
 * If it is, it returns true. Otherwise, it returns false.
 *
 * @param p1 Void pointer to the first ProConBlock to be compared.
 * @param p2 Void pointer to the second ProConBlock to be compared.
 * @return Boolean value indicating whether the total time of the first ProConBlock is less than the total time of the second ProConBlock.
 */
inline static _Bool jobTimeCompare(void *p1, void *p2) {
    return ((ProConBlock *) (p1))->p_total_time < ((ProConBlock *) (p2))->p_total_time;
}

/**
 * @brief Implements the Shortest Job Next scheduling algorithm for a ProConBlockLink.
 *
 * This function sorts the ProConBlocks in a ProConBlockLink based on their total time, from shortest to longest.
 * It uses the jobTimeCompare function as the comparison function for sorting, which compares the total time of two ProConBlocks.
 * The sorting is done by calling the sortLinkFromLinkParam function, which sorts a ProConBlockLink using an algorithm similar to insertion sort.
 * After the function call, the ProConBlock with the shortest total time will be the first ProConBlock in the ProConBlockLink (the one after the head), and the ProConBlock with the longest total time will be the last ProConBlock.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink to be scheduled.
 */
void shortestJobNext(ProConBlockLink *proConBlockLink) {
    sortLinkFromLinkParam(proConBlockLink, jobTimeCompare);
}

/**
 * @brief Executes a ProConBlockLink using specified sorting and execution functions.
 *
 * This function takes a ProConBlockLink and two function pointers as parameters.
 * The first function pointer (proSortFunc) points to a function that sorts the ProConBlocks in the ProConBlockLink.
 * The second function pointer (proExeFunc) points to a function that executes the ProConBlocks in the ProConBlockLink.
 * If proSortFunc is NULL, it defaults to the firstComeFirstServe function.
 * If proExeFunc is NULL, it defaults to the executeOver function.
 * It then calls the sorting function and the execution function in order.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink to be executed.
 * @param proSortFunc Function pointer to the sorting function. If NULL, defaults to firstComeFirstServe.
 * @param proExeFunc Function pointer to the execution function. If NULL, defaults to executeOver.
 */
void runningProConBlockFromLink(
        ProConBlockLink *proConBlockLink,
        void(*proSortFunc)(ProConBlockLink *sortLink),
        void (*proExeFunc)(ProConBlockLink *exeLink)
) {
    // order execute process link once
    proSortFunc = proSortFunc != NULL ? proSortFunc : firstComeFirstServe;
    proExeFunc = proExeFunc != NULL ? proExeFunc : executeOver;
    proSortFunc(proConBlockLink);
    proExeFunc(proConBlockLink);
}

/**
 * @brief Displays the details of all ProConBlocks in a ProConBlockLink.
 *
 * This function iterates over a ProConBlockLink and calls the displayProConBlock function for each ProConBlock.
 * It first prints a line of hashes to the console to indicate the start of the ProConBlockLink.
 * It then retrieves the head ProConBlock of the ProConBlockLink and starts a loop that continues until it has processed all ProConBlocks in the ProConBlockLink.
 * In each iteration of the loop, it calls the displayProConBlock function to display the details of the current ProConBlock, and then moves on to the next ProConBlock.
 * After the loop, it prints another line of hashes to the console to indicate the end of the ProConBlockLink.
 *
 * @param proConBlockLink Pointer to the ProConBlockLink whose ProConBlocks will be displayed.
 */
void displayProConBlockLink(ProConBlockLink *proConBlockLink) {

    printf_s("#############################################################\n");
    ProConBlock *temp = proConBlockLink->headProConBlock;
    while (temp != NULL) {
        displayProConBlock(temp);
        temp = temp->aftProConBlock;
    }
    printf_s("#############################################################\n");
}
