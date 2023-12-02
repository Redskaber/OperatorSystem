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



static ProConBlock *headProConBlock() {
    ProConBlock *head = (ProConBlock *) malloc(sizeof(ProConBlock));
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


ProConBlock *
initProConBlock(int p_id, char *p_name, double p_total_time, ProcessPriority p_priority, CallBack callBack) {
    ProConBlock *newProConBlock = headProConBlock();

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

void destroyProConBlock(ProConBlock *proConBlock) {
    if (proConBlock != NULL) {
        if (proConBlock->aftProConBlock != NULL) {
            destroyProConBlock(proConBlock->aftProConBlock);
        }
        free(proConBlock);
        proConBlock = NULL;

    }
}

void displayProConBlock(ProConBlock *ProConBlock) {
    printf_s("###################################\n");
    printf_s("process:\n");
    printf_s("\tprocess_id: %d\n", ProConBlock->p_id);
    printf_s("\tprocess_name: %s\n", ProConBlock->p_name);
    printf_s("\tprocess_state: %s\n", proStateToString(ProConBlock->p_state));
    printf_s("\tprocess_priority: %s\n", proPriorityToString(ProConBlock->p_priority));
    printf_s("\tprocess_total_time: %f\n", ProConBlock->p_total_time);
    printf_s("\tprocess_execute_time: %f\n", ProConBlock->p_execute_time);
    printf_s("###################################\n");
}


ProConBlockLink *initProConBlockLink() {
    ProConBlockLink *newProConBlockLink = NULL;
    newProConBlockLink = (ProConBlockLink *) malloc(sizeof(ProConBlockLink));
    newProConBlockLink->headProConBlock = headProConBlock();
    newProConBlockLink->lastProConBlock = newProConBlockLink->headProConBlock;

    return newProConBlockLink;
}

void destroyProConBlockLink(ProConBlockLink *proConBlockLink) {
    if (proConBlockLink != NULL) {
        destroyProConBlock(proConBlockLink->headProConBlock);
        free(proConBlockLink);
    }
}

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

inline static void insertProConBlockToOrderLink(
        ProConBlockLink *proConBlockLink, ProConBlock *orderProConBlock, ProConBlock *insertProConBlock) {

    // order         ↓
    // insertIndex   ↓
    // insertData            ↓
    // H        ->  [1] <-> [2] <-> [3] -> NULL
    // 1.脱 插入节点上节点指向下节点，下节点指向上节点

    insertProConBlock->perProConBlock->aftProConBlock = insertProConBlock->aftProConBlock;
    if (insertProConBlock->aftProConBlock != NULL) {
        insertProConBlock->aftProConBlock->perProConBlock = insertProConBlock->perProConBlock;
    } else {
        // 4.末尾值处理，当insert data 是最后的值时， order值一定时last data
        proConBlockLink->lastProConBlock = insertProConBlock->perProConBlock;
    }

    // 2.按 将脱下的节点按到指定节点之后上
    if (orderProConBlock != proConBlockLink->headProConBlock) {
        insertProConBlock->perProConBlock = orderProConBlock;
    } else {
        insertProConBlock->perProConBlock = NULL;
    }

    insertProConBlock->aftProConBlock = orderProConBlock->aftProConBlock;
    orderProConBlock->aftProConBlock->perProConBlock = insertProConBlock;
    orderProConBlock->aftProConBlock = insertProConBlock;
}


void sortLinkFromLinkParam(ProConBlockLink *proConBlockLink, Compare compare) {
    // 下面使用类似数组中的插入排序，一致的思想
    // order         ↓
    // insertIndex   ↓
    // insertData            ↓
    // H        ->  [1] <-> [2] <-> [3] -> NULL

    if (proConBlockLink->headProConBlock->aftProConBlock == NULL ||
        proConBlockLink->headProConBlock->aftProConBlock == proConBlockLink->lastProConBlock)
        return;

    ProConBlock *order = proConBlockLink->headProConBlock->aftProConBlock;
    ProConBlock *insertData = NULL;
    ProConBlock *insertIndex = NULL;

    while (order->aftProConBlock != NULL) {
        insertData = order->aftProConBlock;
        insertIndex = order;

        while (insertIndex != NULL && compare(insertData, insertIndex)) {
            insertIndex = insertIndex->perProConBlock;
        }
        if (insertIndex == NULL) {
            insertIndex = proConBlockLink->headProConBlock;
        }

        if (insertIndex != order) {
            insertProConBlockToOrderLink(proConBlockLink, insertIndex, insertData);
        } else {
            // 3.归 如果没有发生交换，则往后移动一位，否则应当不移动
            order = order->aftProConBlock;
        }

    }
}


void popBlackFromLink(ProConBlockLink *proConBlockLink) {
    if (proConBlockLink->lastProConBlock != NULL) {
        if (proConBlockLink->headProConBlock->aftProConBlock != proConBlockLink->lastProConBlock) {
            // [h] -> [] <-> [] <-> []
            ProConBlock *popProConBlock = proConBlockLink->lastProConBlock;
            ProConBlock *perProConBlock = popProConBlock->perProConBlock;
            destroyProConBlock(popProConBlock);

            perProConBlock->aftProConBlock = NULL;
            proConBlockLink->lastProConBlock = perProConBlock;
        } else {
            // [h] -> []
            destroyProConBlock(proConBlockLink->headProConBlock->aftProConBlock);
            proConBlockLink->headProConBlock->aftProConBlock = NULL;
            proConBlockLink->lastProConBlock = NULL;
        }
    }
}

void popFrontFromLink(ProConBlockLink *proConBlockLink) {
    if (proConBlockLink->headProConBlock->aftProConBlock != NULL) {
        // [h] -> [] <-> [] <-> []
        ProConBlock *aftProConBlock = proConBlockLink->headProConBlock->aftProConBlock->aftProConBlock;
        proConBlockLink->headProConBlock->aftProConBlock->aftProConBlock = NULL;
        destroyProConBlock(proConBlockLink->headProConBlock->aftProConBlock);
        proConBlockLink->headProConBlock->aftProConBlock = aftProConBlock;
        if (aftProConBlock != NULL) {
            aftProConBlock->perProConBlock = NULL;
        }
    }
}

void priorityInsertLink(ProConBlock *proConBlock, ProConBlockLink *proConBlockLink) {
    if (proConBlockLink->headProConBlock->aftProConBlock == NULL) {
        // [] -> []
        proConBlockLink->headProConBlock->aftProConBlock = proConBlock;
        proConBlockLink->lastProConBlock = proConBlock;
    } else {
        // <-[2]->
        // [h] -> [3] <->[2] <-> [1]
        // [h] -> [3] <->[2] <-> [2] <-> [1]
        if (proConBlock->p_priority >= proConBlockLink->headProConBlock->aftProConBlock->p_priority) {
            pushToLink(proConBlock, proConBlockLink);
        } else {
            ProConBlock *temp = proConBlockLink->headProConBlock->aftProConBlock->aftProConBlock;
            while (temp != NULL && temp->p_priority > proConBlock->p_priority) {
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

// sort
void reverseProConBlockFromLink(ProConBlockLink *proConBlockLink) {
    ProConBlock *orderProConBlock = proConBlockLink->headProConBlock->aftProConBlock;
    ProConBlock *insertProConBlock = proConBlockLink->lastProConBlock;
    ProConBlock *oldFirstProConBlock = proConBlockLink->headProConBlock->aftProConBlock;

    if (orderProConBlock != NULL && insertProConBlock != NULL && orderProConBlock == insertProConBlock)
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

void firstComeFirstServe(ProConBlockLink *proConBlockLink) {
    reverseProConBlockFromLink(proConBlockLink);
}

inline static void
insertProConBlockToSamePriorityAfter(ProConBlockLink *proConBlockLink, ProConBlock *slow, ProConBlock *quick) {
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

inline static ProConBlock *
orderProConBlockThroughPriority(ProConBlockLink *proConBlockLink, ProConBlock *slow, ProConBlock *quick,
                                ProcessPriority priority) {
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

// exe
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

inline static ProConBlock *removeNextProConBlockFromLink(ProConBlock *loopLink) {
    loopLink->perProConBlock->aftProConBlock = loopLink->aftProConBlock;
    loopLink->aftProConBlock->perProConBlock = loopLink->perProConBlock;
    return loopLink;
}

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

inline static ProConBlock *
reconfigurationProConBlockLink(ProConBlockLink *proConBlockLink, ProConBlock *finishLink, ProConBlock *cache) {
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

void roundRobinScheduling(ProConBlockLink *proConBlockLink) {
    /*
     * @Params:
     *  ProConBlockLink * :
     *      It is a linked list structure composed of the basic unit of process
     *      task scheduling (process control block (pcb)).
     *
     * @Description:
     *  This function implements the time slice scheduling algorithm (time slice rotation)
     *  in the process task scheduling algorithm.
     */

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

inline static _Bool jobTimeCompare(void *p1, void *p2) {
    return ((ProConBlock *) (p1))->p_total_time < ((ProConBlock *) (p2))->p_total_time;
}

void shortestJobNext(ProConBlockLink *proConBlockLink) {
    sortLinkFromLinkParam(proConBlockLink, jobTimeCompare);
}

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

void displayProConBlockLink(ProConBlockLink *proConBlockLink) {
    printf_s("#############################################################\n");
    ProConBlock *temp = proConBlockLink->headProConBlock;
    while (temp != NULL) {
        displayProConBlock(temp);
        temp = temp->aftProConBlock;
    }
    printf_s("#############################################################\n");
}
