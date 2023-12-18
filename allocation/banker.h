/*
 User: Redskaber
 Date: 2023/12/7
 Time: 15:13
*/
#ifndef OPERATORSYSTEM_BANKER_H
#define OPERATORSYSTEM_BANKER_H
#define BANKER_INIT_ARRAY_MEMBER 5
/*动态资源分配
    银行家算法：
        1.银行家算法（Banker's Algorithm）是一种用于避免死锁（Deadlock）的资源分配算法，特别是在多进程共享有限资源的情况下。
        它是由 Edsger Dijkstra 提出的。银行家算法通过在分配资源之前检查系统状态，判断是否存在安全序列，从而避免了死锁的发生。


        基本概念：
            资源类型（Resource Type):     系统中存在的资源种类，如打印机、内存块等。
            进程（Process):              请求资源的程序或任务。
            最大需求矩阵（Maximum Demand Matrix):      记录每个进程对各类资源的最大需求量。
            可用资源向量（Available Resources Vector): 记录系统当前可用的资源数量。
            分配矩阵（Allocation Matrix): 记录已分配给各进程的资源数量。
            需求矩阵（Need Matrix):       记录各进程还需的资源数量。


        2.实现步骤:
            初始化
            判断安全性
            资源请求检查
            试探分配
            安全性检查
            资源释放
 */
#include <assert.h>
#include "base/resource_allocate.h"
#include "../allocator/systemResource.h"
#include "../process/process_scheduling.h"
#include "../tools/hashMap/hashMap.h"
#include "../tools/hashMapResource/hashMapResource.h"


typedef struct BankProConBlock {
    ProConBlock *base;
    AllocatorResource *resource;
} BankProConBlock;

typedef struct Banker {
    BankProConBlock **array;
    BaseAllocateArr *availableResource;
    int size;
    int maxSize;
} Banker;


extern BankProConBlock *initBankProConBlock(
        int p_id,
        char *p_name,
        double p_total_time,
        ProcessPriority p_priority,
        CallBack callBack,
        Allocator *allocator
);

extern BankProConBlock *initBankProConBlockUsed(ProConBlock *proConBlock, Allocator *allocator);

extern void initAllocatorResourceArr(
        BankProConBlock *bankProConBlock,
        ResourceType maxResourceArr[][2],
        ResourceType assignedResourceArr[][2],
        int rows,
        SystemResource *systemResource
);

extern void displayAllocatorResource(AllocatorResource *allocatorResource);

extern void displayBankProConBlock(BankProConBlock *bankProConBlock);

extern void destroyBankProConBlock(BankProConBlock *bankProConBlock, Allocator *allocator);

extern Banker *initBanker(
        ResourceType availableResourceArr[][2],
        int rows,
        SystemResource *systemResource
);

extern void destroyBanker(Banker *banker, SystemResource *systemResource);

extern void pushProConBlockToBanker(
        Banker *banker,
        BankProConBlock *bankProConBlock,
        SystemResource *systemResource
);

extern void pushProConBlockArrToBanker(
        Banker *banker,
        ProConBlock **pcbArr,
        int member, int rows,
        ResourceType bankerProConBlockGroup[member][rows][3],
        SystemResource *systemResource
);

extern void bankerResourceAllocatedAlgorithm(
        int mp, ProConBlock *pcbArr[mp],
        int mr, ResourceType availableResourceArr[mr][2],
        ResourceType bankerProConBlockGroup[mp][mr][3],
        SystemResource *systemResource
);

extern _Bool
checkResourceSecurity(
        Banker *banker,
        SystemResource *systemResource,
        BankProConBlock *(*orderExecute)[banker->size]
);

#endif //OPERATORSYSTEM_BANKER_H
