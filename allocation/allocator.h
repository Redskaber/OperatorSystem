/*
 User: Redskaber
 Date: 2023/12/7
 Time: 15:13
*/
#ifndef OPERATORSYSTEM_ALLOCATOR_H
#define OPERATORSYSTEM_ALLOCATOR_H

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

        ```
            def banker_algorithm(processes, available_resources, max_demand, allocation):
                # Initialize matrices and vectors

                while True:
                    # Check for an available process that can be satisfied
                    process = find_satisfiable_process()

                    if process is None:
                        break  # No satisfiable process found, exit loop

                    # Try to allocate resources to the process
                    if try_allocation(process):
                        # Successfully allocated resources, release them after some work
                        release_resources(process)

                return "Safe" if all_processes_finished() else "Unsafe"
        ```


        3.结构体设计

            1.PCB
                pid, p_name, state, priority, ...

            2.resource type
                memory, cpu, file, ...

            3.resource manage
                max resource
                need resource
                has resource

            4.resource calc
                allocate...
                deallocate ...
 */
#include <assert.h>
#include "../allocator/memory.h"
#include "../process/process_scheduling.h"


#define DEFAULT_BASE_ALLOCATE 5

typedef int Memory;
typedef int Cpu;
typedef int Gpu;
typedef int Swap;
typedef int NetWork;
typedef int File;


typedef enum ResourceType {
    memory,
    cpu,
    gpu,
    swap,
    network,
    file
} ResourceType;


typedef struct SystemResource {
    Memory memory;
    Cpu cpu;
    Gpu gpu;
    Swap swap;
    NetWork netWork;
    File file;
} SystemResource;


typedef struct BaseAllocate {
    ResourceType type;
    int number;
} BaseAllocate;

typedef struct BaseAllocateArr {
    BaseAllocate **array;
    int max_member;
    int member;
} BaseAllocateArr;

// 动态分配
typedef struct AllocatorResource {
    BaseAllocateArr *maxResource;
    BaseAllocateArr *assignedResource;
    BaseAllocateArr *needResource;
    int size;
} AllocatorResource;


typedef struct BankProConBlock {
    ProConBlock *base;
    AllocatorResource *resource;
} BankProConBlock;


#define resourceTypeToString(type) _Generic((type), \
    enum ResourceType:                              \
        (type == memory) ? "memory":                \
        (type == cpu) ? "cpu":                      \
        (type == gpu) ? "gpu":                      \
        (type == swap) ?"swap":                     \
        (type == network) ? "network":              \
        (type == file) ? "file":"Unknown"           \
)


extern SystemResource *
initSystemResource(Memory m1, Cpu c1, Gpu g1, Swap s1, NetWork n1, File f1, Allocator *allocator);

extern BankProConBlock *initBankProConBlockUsed(ProConBlock *proConBlock, Allocator *allocator);

extern void pushToResourceArr(BaseAllocateArr *destArr, ResourceType resource[2], Allocator *allocator);

extern void
initAllocatorResourceArr(
        BankProConBlock *bankProConBlock,
        ResourceType maxResourceArr[][2],
        ResourceType assignedResourceArr[][2],
        int rows,
        Allocator *allocator
);

extern void destroySystemResource(SystemResource *systemResource, Allocator *allocator);

extern void displaySystemResource(SystemResource *systemResource);

extern BaseAllocate *initBaseAllocate(Allocator *allocator, ResourceType type, int number);

extern void destroyBaseAllocate(BaseAllocate *baseAllocate, Allocator *allocator);

extern void displayBaseAllocate(BaseAllocate *baseAllocate);

extern void destroyBaseAllocateArr(BaseAllocateArr *baseAllocateArr, Allocator *allocator);

extern void destroyAllocatorResource(AllocatorResource *allocatorResource, Allocator *allocator);

extern void displayAllocatorResource(AllocatorResource *allocatorResource);

extern BankProConBlock *
initBankProConBlock(
        int p_id,
        char *p_name,
        double p_total_time,
        ProcessPriority p_priority,
        CallBack callBack,
        Allocator *allocator
);

extern void destroyBankProConBlock(BankProConBlock *bankProConBlock, Allocator *allocator);

extern void displayBankProConBlock(BankProConBlock *bankProConBlock);

#endif //OPERATORSYSTEM_ALLOCATOR_H
