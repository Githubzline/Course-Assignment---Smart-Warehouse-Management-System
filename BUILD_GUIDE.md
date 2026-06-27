# 智能仓储管理系统 — 从零搭建指南

## 搭建顺序（7步，自底向上）

每一步都能独立编译验证，不会到最后才爆一堆错。

---

## 第 1 步：打地基 — 全局常量 + 货物结构体

### 创建文件
- `Common.h`  — 所有 `#define` 常量
- `Goods.h`   — `struct Goods` 结构体

### Common.h 内容
```c
#define MAX_SIZE           100    // 顺序表最大容量
#define MAX_NAME            20    // 名称/类别字符串最大长度
#define MAX_CATEGORY        50    // 最大类别数
#define MAX_GOODS_PER_CAT  100    // 每个类别下最多货物数
#define MAX_LOC             10    // 货位编号最大长度
#define MAX_DATE            11    // 日期 "YYYY-MM-DD" + 结束符
#define MAX_LINE           200    // 文件读取行缓冲区
#define DATA_FILE "warehouse.dat"  // 数据文件名
```

### Goods.h 内容
```c
struct Goods {
    char name[MAX_NAME];       // 货物名称
    int  id;                   // 编号（唯一标识）
    char category[MAX_NAME];   // 所属类别
    char location[MAX_LOC];    // 存储货位编号
    int  quantity;             // 库存数量
    char entryDate[MAX_DATE];  // 入库日期 YYYY-MM-DD
};
```

### 验证
```bash
g++ -c -x c++ Goods.h   # 头文件语法检查
```

---

## 第 2 步：搬教材代码 — 二叉树（BTree）

### 来源
`library/BTree 5.18/BTree.h` + `BTree.cpp`（教材原版）

### 操作
1. 复制到 `finalCode/BTree.h` 和 `BTree.cpp`
2. 只改一处：BTree.cpp 中 `#include <stdlib.h>` 改为 `#include <cstdlib>`
3. 其余一字不改

### 关键点
- `ElemType = int`，正好被后续 CategoryTree 用作 `cats[]` 的下标
- 所有函数通用：`CreateBTree`、`PreOrder`、`InOrder`、`PostOrder`、`LevelOrder`、`BTHeight`、`NodeCount`、`LeafCount`、`DispBTree`、`trans1`、`trans2`

### 验证
```cpp
// test_btree.cpp
#include "BTree.h"
int main() {
    BTNode *bt;
    CreateBTree(bt, "A(B(D,E),C(F,G))");
    printf("先序: "); PreOrder(bt);   // 期望: A B D E C F G
    printf("中序: "); InOrder(bt);    // 期望: D B E A F C G
    printf("后序: "); PostOrder(bt);  // 期望: D E B F G C A
    printf("层序: "); LevelOrder(bt); // 期望: A B C D E F G
    printf("高度: %d\n", BTHeight(bt));   // 期望: 3
    printf("节点: %d\n", NodeCount(bt));  // 期望: 7
    DestroyBTree(bt);
    return 0;
}
```
```bash
g++ -o test_btree.exe test_btree.cpp BTree.cpp && ./test_btree.exe
```

---

## 第 3 步：搬教材代码 — 顺序表（适配 Goods）

### 来源
`library/list 4.13/SqList.h` + `SqList.cpp`（教材原版）

### 操作
1. 复制到 `finalCode/SqList.h` 和 `SqList.cpp`
2. **SqList.h 改动**：
   - 删掉 `typedef int ElemType;`
   - 函数签名中 `ElemType` 全部换成 `Goods`
   - 加入 `#include "Goods.h"`
3. **SqList.cpp 改动**：
   - 函数签名中 `ElemType` 全部换成 `Goods`
   - `DispList` 中 `printf("%d ")` 改为按 `Goods` 字段格式化输出

### 关键坑
BTree.h 已经定义了 `typedef int ElemType`，SqList.h 不能再定义同名 typedef。
所以 SqList 直接用 `Goods` 类型，不通过 `ElemType` 中转。

### 验证
```cpp
// test_sqlist.cpp
#include "SqList.h"
int main() {
    SqList L;
    InitList(L);
    Goods g1 = {"螺丝刀", 1001, "五金", "A01", 50, "2026-06-01"};
    Goods g2 = {"电钻",   1002, "五金", "A02", 30, "2026-06-02"};
    InsElem(L, g1, 1);
    InsElem(L, g2, 2);
    printf("表长: %d\n", GetLength(L));  // 期望: 2
    DispList(L);  // 应打印两行货物，带中文表头
    return 0;
}
```
```bash
g++ -o test_sqlist.exe test_sqlist.cpp SqList.cpp && ./test_sqlist.exe
```

---

## 第 4 步：写邻接矩阵图 — 对照 PPT 第 7 章

### 创建文件
`MatGraph.h` + `MatGraph.cpp`

### PPT 参考
- 结构体 `MatGraph`：PPT 7.2 节「图的存储结构—邻接矩阵」
- 函数 `Dijkstra`：PPT 7.5.1 节 slides 56-57
- 函数 `Floyd`：   PPT 7.5.2 节 slides 68-69

### 结构体（与 PPT 完全一致）
```c
typedef struct {
    char vexs[MAXVEX][MAX_LOC];   // 顶点（货位编号）
    int  edges[MAXVEX][MAXVEX];   // 邻接矩阵（权值 = 距离）
    int  n;                       // 实际顶点数（n==0 表示未初始化）
    int  e;                       // 实际边数
} MatGraph;
```

### 函数清单

| 函数 | 来源 | 说明 |
|------|------|------|
| `InitMatGraph(MatGraph &g)` | 自创 | 初始化：所有边 INF，n=e=0 |
| `InitDemoGraph(MatGraph &g)` | 自创 | 硬编码 2x5 仓库网格（A01~B05） |
| `DestroyMatGraph(MatGraph &g)` | PPT DestroyGraph | 重置 n=e=0 |
| `FindLocation(MatGraph &g, const char *loc)` | 自创 | 货位名 -> 顶点下标 |
| `DispMatGraph(MatGraph &g)` | PPT DispGraph | 显示邻接矩阵 |
| **`Dijkstra(MatGraph &g, const char *start, const char *end)`** | PPT 7.5.1 | 单源最短路径 |
| **`Floyd(MatGraph &g)`** | PPT 7.5.2 | 全源最短路径 |

### Dijkstra 对照 PPT 写
```c
// PPT slide 56: 初始化
for (i=0; i<g.n; i++) {
    dist[i] = g.edges[v][i];
    S[i] = 0;
    if (g.edges[v][i] < INF) path[i] = v;
    else path[i] = -1;
}
S[v] = 1;

// PPT slide 57: 主循环
for (i=0; i<g.n-1; i++) {
    // 选不在 S 中且有最小距离的顶点 u
    mindis = INF;
    for (j=0; j<g.n; j++)
        if (S[j]==0 && dist[j]<mindis) { u=j; mindis=dist[j]; }
    S[u] = 1;
    // 用 u 更新其他顶点
    for (j=0; j<g.n; j++)
        if (S[j]==0 && g.edges[u][j]<INF && dist[u]+g.edges[u][j]<dist[j]) {
            dist[j] = dist[u] + g.edges[u][j];
            path[j] = u;
        }
}
```

### Floyd 对照 PPT 写
```c
// PPT slide 68: 初始化
for (i=0; i<g.n; i++)
    for (j=0; j<g.n; j++) {
        A[i][j] = g.edges[i][j];
        if (i!=j && g.edges[i][j]<INF) path[i][j] = i;
        else path[i][j] = -1;
    }

// PPT slide 69: 三重循环
for (k=0; k<g.n; k++)
    for (i=0; i<g.n; i++)
        for (j=0; j<g.n; j++)
            if (A[i][j] > A[i][k] + A[k][j]) {
                A[i][j] = A[i][k] + A[k][j];
                path[i][j] = path[k][j];
            }
```

### 验证
```cpp
// test_graph.cpp
#include "MatGraph.h"
int main() {
    MatGraph g;
    InitDemoGraph(g);
    DispMatGraph(g);                    // 看 10x10 邻接矩阵
    Dijkstra(g, "A01", "B05");         // 看最短路径
    Floyd(g);                           // 看全源最短距离矩阵
    return 0;
}
```
```bash
g++ -o test_graph.exe test_graph.cpp MatGraph.cpp && ./test_graph.exe
```

---

## 第 5 步：搭业务层 — GoodsManager + CategoryTree

### GoodsManager（GoodsManager.h/.cpp）

封装 SqList，加上业务逻辑：

| 函数 | 说明 |
|------|------|
| `GM_Init(L)` | 初始化顺序表 |
| `GM_GetById(L, id, g)` | 按编号查找货物 |
| `GM_FindPos(L, id)` | 按编号找位置（顺序查找 O(n)） |
| `GM_Add(L, g)` | 入库：检查 ID 重复 + 尾插 |
| `GM_Delete(L, id)` | 出库：按 ID 删除 |
| `GM_Update(L, id, g)` | 修改：检查新 ID 冲突 + 覆盖 |
| `GM_Count(L)` | 返回货物总数 |
| `GM_DisplayAll(L)` | 打印全部货物 |

### CategoryTree（CategoryTree.h/.cpp）

本项目最精巧的设计：**BST + 旁路数组 混合索引**

```
BST 做什么：快速查找类别（O(log n)），节点存的是 cats[] 下标
旁路数组做什么：实际存储类别名 + 该类别下所有货物 ID

        办公用品(idx=3)
         /            \
      电子(idx=1)    食品(idx=2)
       /
    五金(idx=0)

cats[]:
  [0] "五金" -> ids:[1001,1002,1003]
  [1] "电子" -> ids:[2001,2002,2003]
  [2] "食品" -> ids:[3001,3002]
  [3] "办公用品" -> ids:[4001,4002]
```

| 函数 | 说明 |
|------|------|
| `CT_Init(ct)` | root=NULL, catCount=0 |
| `CT_FindCategory(ct, name)` | 遍历 cats[] 查找类别 |
| `CT_AddCategory(ct, name)` | cats[] 追加 + BST 插入新节点 |
| `CT_InsertGoods(ct, idx, goodsId)` | 在 cats[idx].goodsIds[] 追加 |
| `CT_RemoveGoods(ct, idx, goodsId)` | 从 cats[idx].goodsIds[] 移除 |
| `CT_DisplayByCategory(ct, L)` | 中序遍历 BST（自动字典序输出） |
| `CT_DisplayStats(ct, L)` | 各类别货物种类数 + 总库存 |

### BST 插入关键代码
```c
// 创建 BST 节点，data = cats[] 下标
BTNode *newNode = malloc(sizeof(BTNode));
newNode->data = idx;  // 存下标，不是类别名！

// BST 插入：比较的是 cats[下标].name
BTNode *p = ct.root, *parent = NULL;
while (p != NULL) {
    parent = p;
    if (strcmp(name, ct.cats[p->data].name) < 0)
        p = p->lchild;   // 新类别名更小 -> 走左边
    else
        p = p->rchild;   // 新类别名更大 -> 走右边
}
// 挂在 parent 下
if (strcmp(name, ct.cats[parent->data].name) < 0)
    parent->lchild = newNode;
else
    parent->rchild = newNode;
```

### 验证
```cpp
CategoryTree ct; SqList L;
CT_Init(ct); InitList(L);
CT_AddCategory(ct, "五金");
CT_AddCategory(ct, "电子");
CT_AddCategory(ct, "食品");
CT_DisplayStats(ct, L);  // 中序输出 -> 电子、食品、五金（字典序）
```

---

## 第 6 步：搭服务层 + 文件管理 + 菜单

### FileManager（FileManager.h/.cpp）

**文件格式**：纯文本，竖线分隔
```
1001|螺丝刀|五金|A01|50|2026-06-01
1002|电钻|五金|A02|30|2026-06-02
```

| 函数 | 说明 |
|------|------|
| `FM_Save(L)` | 遍历 SqList，fprintf 逐行写入 warehouse.dat |
| `FM_Load(L, ct)` | fgets 逐行读，按 `|` 分割解析，调 GM_Add + CT_AddCategory 重建索引 |

### WarehouseSystem（WarehouseSystem.h/.cpp）

持有三个数据结构，负责**跨结构数据同步**：
```c
struct WarehouseSystem {
    SqList        goodsList;   // 顺序表：所有货物
    CategoryTree  catTree;     // BST+旁路：类别索引
    MatGraph      layout;      // 邻接矩阵：仓库布局
};
```

| 函数 | 跨结构操作 |
|------|-----------|
| `WS_AddGoods` | ① GM_Add(顺序表) ② CT_AddCategory(BST) ③ CT_InsertGoods(旁路) |
| `WS_DeleteGoods` | ① CT_RemoveGoods(解关联) ② GM_Delete(删数据) |
| `WS_UpdateGoods` | 如类别变了：旧类别移除 + 新类别添加 + 更新数据 |
| `WS_ShortestPath` | ① GM_GetById(顺序表查货位) ② Dijkstra(图算路径) |
| `WS_BrowseByCategory` | 中序遍历 BST -> cats[] 取 ID 列表 -> GM_GetById 查详情 |
| `WS_LoadDemo` | 10 条演示货物，覆盖 4-5 个类别 |

### Menu（Menu.h/.cpp）

| 组件 | 说明 |
|------|------|
| `ReadInt` / `ReadString` | fgets + sscanf 安全输入（不用 scanf） |
| `ShowMainMenu` | 打印 11 个选项 |
| `DoAddGoods` / `DoDeleteGoods` / ... | 每个菜单项一个 static 函数 |
| `Menu_Run` | 主循环：do { 显示菜单 -> 读选择 -> 执行 } while(choice!=0) |
| 全局 `ws` | `static WarehouseSystem ws`（只在 Menu.cpp 内可见） |

### main.cpp（入口，两行）
```cpp
int main(int argc, char** argv) {
    Menu_Run();
    return 0;
}
```

### 验证
```bash
g++ -o WareHouse.exe main.cpp Menu.cpp WarehouseSystem.cpp \
    GoodsManager.cpp SqList.cpp BTree.cpp CategoryTree.cpp \
    MatGraph.cpp FileManager.cpp
./WareHouse.exe
# 依次测试 1~11 号菜单，最后选 0 退出
```

---

## 第 7 步：加 Demo 数据

在 `WS_LoadDemo` 中放入 10 条示例货物：

```c
Goods demo[] = {
    {"螺丝刀", 1001, "五金",     "A01", 50,  "2026-06-01"},
    {"电钻",   1002, "五金",     "A02", 30,  "2026-06-02"},
    {"扳手",   1003, "五金",     "A03", 20,  "2026-06-03"},
    {"芯片",   2001, "电子",     "B01", 200, "2026-06-01"},
    {"电阻",   2002, "电子",     "B02", 500, "2026-06-02"},
    {"电容",   2003, "电子",     "B03", 1000,"2026-06-03"},
    {"矿泉水", 3001, "食品",     "A04", 100, "2026-06-05"},
    {"饼干",   3002, "食品",     "A05", 200, "2026-06-05"},
    {"打印纸", 4001, "办公用品", "B04", 80,  "2026-06-04"},
    {"签字笔", 4002, "办公用品", "B05", 300, "2026-06-04"},
};
```

---

## 依赖关系总图

```
Common.h ---- Goods.h ---- SqList.h/.cpp ---- GoodsManager
    │                                            │
    └---- BTree.h/.cpp ---- CategoryTree         │
              │              (BST+旁路数组)       │
              │                                  │
    └---- MatGraph.h/.cpp                        │
              │                                  │
              └----------------------------------│
                                                 ▼
                                    WarehouseSystem (服务层/协调层)
                                          │
                                    FileManager (持久化)
                                          │
                                    Menu.cpp (UI层)
                                          │
                                    main.cpp (入口)
```

## 文件清单（最终 19 个源文件）

```
Common.h           全局常量
Goods.h            货物结构体
BTree.h / BTree.cpp           二叉树（教材原版，一字未改）
SqList.h / SqList.cpp         顺序表（教材原版 + Goods 适配）
MatGraph.h / MatGraph.cpp     邻接矩阵图（对照 PPT 第 7 章）
GoodsManager.h / GoodsManager.cpp   货物 CRUD 封装
CategoryTree.h / CategoryTree.cpp   BST+旁路 类别索引
FileManager.h / FileManager.cpp     文件读写
WarehouseSystem.h / WarehouseSystem.cpp  跨结构协调
Menu.h / Menu.cpp               终端菜单
main.cpp                        程序入口
```

## 编译命令（全量）

```bash
g++ -o WareHouse.exe main.cpp Menu.cpp WarehouseSystem.cpp \
    GoodsManager.cpp SqList.cpp BTree.cpp CategoryTree.cpp \
    MatGraph.cpp FileManager.cpp
```
