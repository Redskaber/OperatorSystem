# OperatorSystem
学习操作系统相关记录

## 进程调度

  **执行流程：**
```markdown
+---------------------+         +----------------------+
|       创建进程       |         |      进程终止        |
+----------+----------+         +----------+-----------+
            |                              ^
            v                              |
+----------+----------+         +----------+-----------+
|       进程就绪       |         |       进程终止       |
| 被唤醒或新创建的进程  |         |      进程执行完成     |
+----------+----------+         +----------+-----------+
           |                               ^
           v                               |
+----------+----------+         +----------+-----------+
|      进程调度        |         |      进程唤醒        |
| 选择下一个要执行的进程|         |    等待的事件发生     |
+----------+----------+         +----------+-----------+
           |                               ^
           v                               |
+----------+----------+         +----------+-----------+
|      进程执行        |         | 进程阻塞或等待        |
|      执行一段时间片  |          | 等待 I/O 操作等      |
+----------+----------+         +----------------------+
```
  **进程调度算法：**
  - 先来先服务 (First Come, First Serve, FCFS): 顺序
  - 短作业优先 (Shortest Job Next, SJN): Min(执行时间)
  - 优先级调度: Max(优先级)
  - 轮转调度 (Round Robin, RR): Loop(运行时间)
  - 多级反馈队列调度: Max【优先级(多级队列 + 队列间优先级)】
  - 最高响应比优先 (Highest Response Ratio Next, HRRN): Min【响应比:（等待时间 + 服务时间）/ 服务时间】
  - 多处理器调度: More(处理机{进程...}) A SMP / SMP
  - 最短剩余时间优先 (Shortest Remaining Time Next, SRTN): Min(剩余时间)
  - 实时调度: 动态

  **结构体设计：**
  PCB
  结构体字段名：

  - 进程ID
  - 进程名
  - 进程状态
  - 进程优先级
  - 进程执行时间
  - 进程总需时间
  - 进程队列
- ```markdown
  进程ID            进程之间的身份标识
  进程名             用于存储进程的名称
  进程状态           用于表示进程的状态    'w'（就绪状态） 和  'R'（运行状态）
    
  进程优先级          用于表示进程优先级
  进程执行时间        表示进程已经运行的时间
  进程总需时间        表示进程需要运行总时间、
    
  进程队列           表示需要执行的进程队列
  ```



## 资源分配

### 银行家算法

银行家算法（Banker's Algorithm）是一种用于避免死锁（Deadlock）的资源分配算法，特别是在多进程共享有限资源的情况下。
它是由 Edsger Dijkstra 提出的。银行家算法通过在分配资源之前检查系统状态，判断是否存在安全序列，从而避免了死锁的发生。

**基本功能:**

- 资源类型（Resource Type):     系统中存在的资源种类，如打印机、内存块等。
- 进程（Process):                      请求资源的程序或任务。
- 最大需求矩阵（Maximum Demand Matrix):      记录每个进程对各类资源的最大需求量。
- 可用资源向量（Available Resources Vector):    记录系统当前可用的资源数量。
- 分配矩阵（Allocation Matrix): 记录已分配给各进程的资源数量。
- 需求矩阵（Need Matrix):       记录各进程还需的资源数量。

**实现步骤:**

- 初始化
- 判断安全性
- 资源请求检查
- 试探分配
- 安全性检查
- 资源释放

## 工具结构

### hashMap

普通的hashMap

**基本功能：**

- 创建： createHashMap
- 插入： insert
- 读取： get
- 销毁： destroyHashMap

```C
typedef struct HashNode {
    char *key;
    int value;
    struct HashNode *next;
} HashNode;

typedef struct HashMap {
    int size;
    HashNode **table;
} HashMap;
```



### hashMapResource

存储指针的hashMap

**基本功能：**

- 创建： createHashMapResource
- 插入： insertResource
- 读取： getResource
- 销毁： destroyHashMapResource

```c
typedef struct HashNodeResource {
    char *key;
    int *value;
    struct HashNodeResource *next;
} HashNodeResource;

typedef struct HashMapResource {
    int size;
    HashNodeResource **table;
} HashMapResource;
```
