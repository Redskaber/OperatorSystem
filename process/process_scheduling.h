/*
 User: Redskaber
 Date: 2023/11/28
 Time: 10:51
*/
#pragma once
#ifndef OPERATORSYSTEMALGORITHM_PROCESS_SCHEDULING_H
#define OPERATORSYSTEMALGORITHM_PROCESS_SCHEDULING_H
/*
 * 进程调度
    执行流程：
        +---------------------+             +----------------------+
        |  创建进程             |             | 进程终止              |
        +----------+----------+             +----------+-----------+
                   |                                ^
                   v                                |
        +----------+----------+             +----------+-----------+
        | 进程就绪             |             | 进程终止              |
        | 被唤醒或新创建的进程   |              | 进程执行完成           |
        +----------+----------+             +----------+-----------+
                   |                                ^
                   v                                |
        +----------+----------+             +----------+-----------+
        | 进程调度             |             | 进程唤醒              |
        | 选择下一个要执行的进程 |              | 等待的事件发生         |
        +----------+----------+             +----------+-----------+
                   |                                ^
                   v                                |
        +----------+----------+             +----------+-----------+
        | 进程执行             |             | 进程阻塞或等待          |
        | 执行一段时间片        |              | 等待 I/O 操作等       |
        +----------+----------+             +----------------------+

    进程调度算法：
        - 先来先服务 (First Come, First Serve, FCFS): 顺序
        - 短作业优先 (Shortest Job Next, SJN): Min(执行时间)
        - 优先级调度: Max(优先级)
        - 轮转调度 (Round Robin, RR): Loop(运行时间)
        - 多级反馈队列调度: Max【优先级(多级队列 + 队列间优先级)】
        - 最高响应比优先 (Highest Response Ratio Next, HRRN): Min【响应比:（等待时间 + 服务时间）/ 服务时间】
        - 多处理器调度: More(处理机{进程...}) A SMP / SMP
        - 最短剩余时间优先 (Shortest Remaining Time Next, SRTN): Min(剩余时间)
        - 实时调度: 动态

    结构体设计：
        PCB
        结构体字段名：
            - 进程ID
            - 进程名
            - 进程状态

            - 进程优先级
            - 进程执行时间
            - 进程总需时间

            - 进程队列

        进程ID            进程之间的身份标识
        进程名             用于存储进程的名称
        进程状态           用于表示进程的状态    'w'（就绪状态） 和  'R'（运行状态）

        进程优先级          用于表示进程优先级
        进程执行时间        表示进程已经运行的时间
        进程总需时间        表示进程需要运行总时间、

        进程队列           表示需要执行的进程队列
 */

#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>

#define TIME_SLICE  5

typedef void *(*CallBack)(void *args);

typedef _Bool (*Compare)(void *a, void *b);

// 进程状态
typedef enum ProcessState {
    new,
    ready,
    running,
    blocked,
    waiting,
    terminated,
    suspended_ready,
    suspended_blocked,
} ProcessState;

// 进程优先级
typedef enum ProcessPriority {
    low,
    normal,
    high,
    exigency
} ProcessPriority;


// 基本配置(进程控制块)
typedef struct ProcessControlBlock {
    int p_id;
    char *p_name;
    ProcessState p_state;

    ProcessPriority p_priority;
    double p_execute_time;
    double p_total_time;

    CallBack callback;

    struct ProcessControlBlock *perProConBlock;
    struct ProcessControlBlock *aftProConBlock;
} ProConBlock;


// 进程控制链(link)
typedef struct ProcessControlBlackLink {
    ProConBlock *headProConBlock;
    ProConBlock *lastProConBlock;
} ProConBlockLink;


#define proStateToString(state) _Generic((state), \
    enum ProcessState:                            \
        (state == new) ? "new" :                  \
        (state == ready) ? "ready" :              \
        (state == running) ? "running" :          \
        (state == blocked) ? "blocked" :          \
        (state == waiting) ? "waiting" :          \
        (state == terminated) ? "terminated" :    \
        (state == suspended_ready) ? "suspended_ready" :               \
        (state == suspended_blocked) ? "suspended_blocked" : "UNKNOWN" \
)

#define proPriorityToString(priority) _Generic((priority), \
    enum ProcessPriority:                                  \
        (priority == low) ? "low":                          \
        (priority == normal) ? "normal":                    \
        (priority == high) ? "high":                        \
        (priority == exigency) ? "exigency": "UNKNOWN"      \
)


extern ProConBlock *
initProConBlock(int p_id, char *p_name, double p_total_time, ProcessPriority p_priority, CallBack callBack);

extern void destroyProConBlock(ProConBlock *proConBlock);

extern void displayProConBlock(ProConBlock *proConBlock);


extern ProConBlockLink *initProConBlockLink();

extern void pushToLink(ProConBlock *proConBlock, ProConBlockLink *proConBlockLink);

extern void popBlackFromLink(ProConBlockLink *proConBlockLink);

extern void popFrontFromLink(ProConBlockLink *proConBlockLink);

extern void displayProConBlockLink(ProConBlockLink *proConBlockLink);

extern void destroyProConBlockLink(ProConBlockLink *proConBlockLink);

extern void priorityInsertLink(ProConBlock *proConBlock, ProConBlockLink *proConBlockLink);

extern void sortLinkFromLinkParam(ProConBlockLink *proConBlockLink, Compare compare);

extern void priorityScheduling(ProConBlockLink *proConBlockLink);

extern void runningProConBlockFromLink(
        ProConBlockLink *proConBlockLink,
        void (*proSortFunc)(ProConBlockLink *sortLink),
        void (*proExeFunc)(ProConBlockLink *exeLink)
);

extern void reverseProConBlockFromLink(ProConBlockLink *proConBlockLink);

extern void firstComeFirstServe(ProConBlockLink *proConBlockLink);

extern ProConBlock *runningProConBlockTask(ProConBlock *loopLink);

extern void roundRobinScheduling(ProConBlockLink *proConBlockLink);

extern void shortestJobNext(ProConBlockLink *proConBlockLink);

#endif //OPERATORSYSTEMALGORITHM_PROCESS_SCHEDULING_H
