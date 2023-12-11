/*
 User: Redskaber
 Date: 2023/12/10
 Time: 17:28
*/
#include <iostream>
//
using std::cout;
using std::cin;
using std::endl;

//定义全局变量
const int x = 10, y = 10;   //常量，便于修改
int Available[x];      //各资源可利用的数量
int Allocation[y][y];  //各进程当前已分配的资源数量
int Max[y][y];  //各进程对各类资源的最大需求数
int Need[y][y]; //尚需多少资源
int Request[x]; //申请多少资源
int Work[x]; //工作向量，表示系统可提供给进程继续运行所需的各类资源数量
int Finish[y]; //表示系统是否有足够的资源分配给进程，1为是,0为否
int p[y];  //存储安全序列
int i, j;   //i表示进程，j表示资源
int n, m;  //n为进程i的数量,m为资源j种类数
int l = 0;   //l用来记录有几个进程是Finish[i]=1的，当l=n是说明系统状态是安全的
int counter = 0;

//函数声明
void chushihua();   //初始化函数
void safe();      //安全性算法
void show();    //函数show,输出当前状态
void bank();     //银行家算法
void jieshu();      //结束函数
void chushihua() {
    cout << "输入进程的数量: ";//从此开始输入有关数据
    cin >> n;
    cout << "输入资源种类数: ";
    cin >> m;
    cout << endl << "输入各种资源当前可用的数量( " << m << " 种): " << endl;
    for (j = 0; j < m; j++) {
        cout << "输入资源 " << j << " 可利用的数量Available[" << j << "]: ";
        cin >> Available[j]; //输入数字的过程...
        Work[j] = Available[j];      //初始化Work[j]，它的初始值就是当前可用的资源数
    }

    cout << endl << "输入各进程当前已分配的资源数量Allocation[" << n << "][" << m << "]: " << endl;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            cout << "     输入进程 " << i << " 当前已分配的资源 " << j << " 数量: ";
            cin >> Allocation[i][j];
        }
        cout << endl;
        Finish[i] = 0;//初始化Finish[i]
    }

    cout << endl << "输入各进程对各类资源的最大需求Max[" << n << "][" << m << "]: " << endl;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            cout << "   输入进程 " << i << " 对资源 " << j << " 的最大需求数: ";
            cin >> Max[i][j];
            if (Max[i][j] >= Allocation[i][j]) //若最大需求大于已分配，则计算需求量
                Need[i][j] = Max[i][j] - Allocation[i][j];
            else
                Need[i][j] = 0;//Max小于已分配的时候，此类资源已足够不需再申请
        }
        cout << endl;
    }
    cout << endl << "初始化完成" << endl;
}

//安全性算法函数
void safe() {
    l = 0;//l用来记录有几个进程是Finish[i]=1的
    for (i = 0; i < n; i++) {  //i++
        if (Finish[i] == 0) {  //逐个查找Finish[i]==0的进程    条件一
            counter = 0;   //记数器
            for (j = 0; j < m; j++) {
                if (Work[j] >= Need[i][j])
                    counter = counter + 1;//
            }

            if (counter == m)  //i进程的每类资源都符合Work[j]>=Need[i][j] 条件二
            {
                p[l] = i;  //存储安全序列
                Finish[i] = 1;  //i进程标志为可分配
                for (j = 0; j < m; j++)
                    Work[j] = Work[j] + Allocation[i][j];  //释放资源
                l = l + 1; //记数,现在有L个进程是安全的，当L=N时说明满足安全序列
                i = -1; //从第一个进程开始继续寻找满足条件一二的进程
            }
        }
    }
}

//显示当前状态函数
void show() //函数show,输出当前资源分配情况
{
    int i, j; //局部变量
    int All[y]; //各种资源的总数量
    int L1; //局部变量L1
    cout << "当前的状态为：" << endl;
    cout << "各种资源的总数量:" << endl;
    for (j = 0; j < m; j++) {
        cout << " 资源" << j << ": ";
        All[j] = Available[j];
        for (i = 0; i < n; i++)
            All[j] += Allocation[i][j];  //总数量=可用的+已分配的
        cout << All[j] << "  ";
    }
    cout << endl << "当前各种资源可用的量为(available):" << endl;
    for (j = 0; j < m; j++)
        cout << " 资源" << j << ": " << Available[j] << "  ";
    cout << endl << "各进程已经得到的资源量(allocation): " << endl;
    for (i = 0; i <= m; i++) {
        for (j = i; j < m; j++)
            cout << "       资源" << j;
        cout << endl;
        for (L1 = 0; L1 < n; L1++) {
            cout << "进程" << L1 << ":";
            for (j = i; j < m; j++)
                cout << Allocation[L1][j] << "          ";
            cout << endl;
        }
    }
    cout << endl << "各进程还需要的资源量(need):" << endl;
    for (i = 0; i <= m; i++) {
        for (j = i; j < m; j++)
            cout << "      资源" << j;
        cout << endl;
        for (L1 = 0; L1 < n; L1++) {
            cout << "进程" << L1 << ":";
            for (j = i; j < m; j++)
                cout << Need[L1][j] << "          ";
            cout << endl;
        }
    }
}

//银行家算法函数
void bank() {
    cout << endl << "进程申请分配资源：" << endl;
    int k = 0;  //用于输入进程编号
    bool r = false;  // 初值为假，输入Y继续申请则置为真
    do {//输入请求
        cout << "输入申请资源的进程(0-" << n - 1 << "): ";
        cin >> k;
        cout << endl;
        while (k > n - 1) //输入错误处理
        {
            cout << endl << "输入错误，重新输入：" << endl;
            cout << endl << "输入申请资源的进程(0--" << n - 1 << "): ";
            cin >> k;
            cout << endl;
        }
        cout << endl << "输入该进程申请各类资源的数量: " << endl;
        for (j = 0; j < m; j++) {
            do {  //do……while 循环判断申请输入的情况
                cout << "进程 " << k << " 申请资源[" << j << "]的数量:";
                cin >> Request[j];
                cout << endl;
                if (Request[j] > Need[k][j]) {  //申请大于需求量时出错，提示重新输入（贷款数目不允许超过需求数目）
                    cout << "申请大于需要量!" << endl;
                    cout << "申请的资源" << j << "的数量为" << Request[j] << ",大于进程" << k << "对该资源需求量"
                         << Need[k][j] << "。" << endl;
                    cout << "重新输入!" << endl;
                } else   //先判断是否申请大于需求量，再判断是否申请大于可利用量
                if (Request[j] > Available[j]) {  //申请大于可利用量， 应该阻塞等待
                    cout << "\n没有那么多资源，目前可利用资源" << j << "数量为" << Available[j] << ",本次申请不成功，进程等待!"
                         << endl;
                    Finish[k] = 0;  //该进程等待
                    goto ppp;  //goto语句 跳转，结束本次申请
                }
            } while (Request[j] > Need[k][j]);  //Request[j]>Available[j]||
        }
        //改变Avilable、Allocation、Need的值
        for (j = 0; j < m; j++) {
            Available[j] = Available[j] - Request[j];
            Allocation[k][j] = Allocation[k][j] + Request[j];
            Need[k][j] = Need[k][j] - Request[j];
            Work[j] = Available[j];
        }
        //判断当前状态的安全性
        safe();  //调用安全性算法函数
        if (l < n) {
            l = 0;
            cout << "\n试分配后，状态不安全,所以不予分配!恢复原状态" << endl;
            //恢复数据
            for (j = 0; j < m; j++) {
                Available[j] = Available[j] + Request[j];
                Allocation[k][j] = Allocation[k][j] - Request[j];
                Need[k][j] = Need[k][j] + Request[j];
                Work[j] = Available[j];
            }
            for (i = 0; i < n; i++)
                Finish[i] = 0;  //进程置为未分配状态
        } else {
            l = 0;
            cout << "\n申请资源成功!!!" << endl;
            for (j = 0; j < m; j++) {
                if (Need[k][j] == 0);
                else {     //有一种资源还没全部申请到，则该进程不可执行，不能释放拥有的资源
                    l = 1;   //置l为1，作为判断标志
                    break;
                }
            }
            if (l != 1) { //进程可以执行，则释放该进程的所有资源
                for (j = 0; j < m; j++) {
                    Available[j] = Available[j] + Allocation[k][j];
                    Allocation[k][j] = 0;
                }
                cout << "该进程已得到所有需求资源，执行后将释放其所有拥有资源！" << endl;
            }
            l = 0;  //归零
            cout << "\n安全的状态!" << endl;
            cout << "安全序列为:   ";
            cout << endl << "进程" << "(" << p[0] << ")";  //输出安全序列，考虑显示格式，先输出第一个
            Finish[0] = 0;
            for (i = 1; i < n; i++) {
                cout << "==>>" << "进程" << "(" << p[i] << ")";
                Finish[i] = 0; //所有进程置为未分配状态
            }
            cout << endl << endl;
        }
        show();  //显示当前状态
        ppp:   //申请大于可利用量， 应该阻塞等待,结束本次资源申请，GOTO 语句跳转至此
        cout << endl << "是否继续申请资源(y/n) ?";
        char *b = new char;  //输入y/n，判断是否继续申请    << endl
        cin >> *b;
        cout << endl;
        cout << "-------------------------------------------" << endl << endl;
        cout << endl;
        if (*b == 'y' || *b == 'Y')
            r = true;
        else {
            r = false;  //输入非 Y 则令 R =false
            jieshu();  //调用结束函数
        }
    } while (r == true);
}

//结束函数
void jieshu() {
    cout << endl << endl;
    cout << "\t\t 演示计算完毕" << endl;
    cout << endl << endl;
}

//主函数
int teacher_allocator() {
    system("chcp 65001");

    cout << endl << endl << "\t\t\t\t模拟银行家算法" << endl << endl;
    chushihua(); //初始化函数调用
    cout << endl;
    show();  //输出当前状态
    safe();  //判断当前状态的安全性
    if (l < n)   //l在safe中是用来记录安全的进程的个数的
    {
        cout << "\n当前状态不安全，拒绝申请！" << endl;
        cout << endl;
        return 0;
    } else {
        int i;  //局部变量
        l = 0;
        cout << endl << "\n当前的状态是安全的!安全序列为:" << endl;
        cout << "进程" << "(" << p[0] << ")";  //输出安全序列
        for (i = 1; i < n; i++) cout << "->>" << "进程" << "(" << p[i] << ")";
        for (i = 0; i < n; i++)
            Finish[i] = 0;  //所有进程置为未分配状态
        cout << endl;
    }

    bank();    //调用银行家算法函数
    cout << "\t\t 演示计算完毕" << endl;
    return 0;
}
