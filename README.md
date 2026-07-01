# 智能仓储管理系统 🏭

> 数据结构课程设计项目 | C++ · 终端应用

基于三种经典数据结构（顺序表、二叉排序树、邻接矩阵图）构建的仓储管理系统，支持货物 CRUD、类别索引、最短路径规划与文件持久化。

---

## 功能

| 模块 | 功能 |
|------|------|
| 货物管理 | 入库、出库、修改、查询（按编号） |
| 类别索引 | BST 二叉排序树 + 旁路数组，中序遍历自动字典序输出 |
| 路径规划 | Dijkstra 最短路径算法，计算仓库内两货位间最短距离 |
| 文件持久化 | 启动自动加载 `warehouse.dat`，退出自动保存 |
| 演示数据 | 内置 10 条货物（五金/电子/食品/办公用品，2×5 网格仓库） |

## 数据结构

```
┌─────────────┐    ┌──────────────────┐    ┌──────────────┐
│   SqList    │    │   CategoryTree    │    │   MatGraph   │
│  顺序表     │    │  BST + 旁路数组   │    │  邻接矩阵图  │
│ 货物CRUD    │    │  类别索引        │    │  Dijkstra    │
└──────┬──────┘    └────────┬─────────┘    └──────┬───────┘
       │                    │                     │
       └────────────────────┼─────────────────────┘
                            ▼
                  ┌─────────────────┐
                  │ WarehouseSystem │  ← 服务层（跨结构同步）
                  └────────┬────────┘
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
        FileManager      Menu        main.cpp
        (持久化)        (终端UI)      (入口)
```

- **SqList（顺序表）**：存储全部货物，支持增删改查
- **CategoryTree（BST + 旁路）**：BST 节点存类别下标，旁路数组 `cats[]` 存类别名和货物 ID 列表；中序遍历自动按字典序输出
- **MatGraph（邻接矩阵图）**：2×5 网格仓库布局，Dijkstra 计算任意两货位间最短路径

## 编译 & 运行

**环境**：MinGW GCC（Dev-C++）或任意支持 C++11 的 g++

```bash
# 全量编译
g++ -o WareHouse.exe main.cpp Menu.cpp WarehouseSystem.cpp \
    GoodsManager.cpp SqList.cpp BTree.cpp CategoryTree.cpp \
    MatGraph.cpp FileManager.cpp

# 运行
./WareHouse.exe
```

也可用 Dev-C++ 打开 `MyWareHouse.dev` 直接编译运行。

## 文件结构

```
├── Common.h              # 全局常量
├── Goods.h               # 货物结构体
├── SqList.h / .cpp       # 顺序表（适配 Goods）
├── BTree.h / .cpp        # 二叉树（教材原版）
├── MatGraph.h / .cpp     # 邻接矩阵图 + Dijkstra
├── GoodsManager.h / .cpp # 货物 CRUD 封装
├── CategoryTree.h / .cpp # BST + 旁路数组 类别索引
├── FileManager.h / .cpp  # 文件持久化
├── WarehouseSystem.h / .cpp # 服务层（跨结构协调）
├── Menu.h / .cpp         # 终端菜单
├── main.cpp              # 入口
├── Makefile.win          # Dev-C++ Makefile
├── MyWareHouse.dev       # Dev-C++ 项目文件
├── BUILD_GUIDE.md        # 从零搭建指南
├── TEAM_PLAN.md          # 团队分工方案
├── reports/              # 设计文档 & 流程图
└── LICENSE               # MIT
```

## 菜单功能

```
1.  入库      6.  全量浏览    11. 最短路径
2.  出库      7.  按类别浏览   0.  退出
3.  修改      8.  类别统计
4.  单条查询  9.  加载演示数据
5.  单条查询  10. 保存
```

## 许可

[MIT](LICENSE) © 2026 Githubzline

---

# Smart Warehouse Management System 🏭

> Data Structures Course Project | C++ · Terminal Application

A warehouse management system built on three classic data structures — sequential list, binary search tree, and adjacency matrix graph. Supports goods CRUD, category-based indexing, shortest-path routing, and file persistence.

## Features

| Module | Description |
|--------|-------------|
| Goods Management | Add, delete, update, query goods (by ID) |
| Category Index | BST + side-array hybrid index; in-order traversal yields dictionary-order output |
| Path Planning | Dijkstra shortest path between any two warehouse locations |
| File Persistence | Auto-load `warehouse.dat` on startup, auto-save on exit |
| Demo Data | 10 built-in items across 4 categories (Hardware / Electronics / Food / Office Supplies on a 2×5 grid) |

## Data Structures

- **SqList (Sequential List)** — stores all goods; supports CRUD operations
- **CategoryTree (BST + Side Array)** — BST nodes hold index into `cats[]` array, which stores category names and goods ID lists; in-order traversal prints in dictionary order
- **MatGraph (Adjacency Matrix)** — 2×5 grid warehouse layout; Dijkstra computes shortest path between any two locations

## Build & Run

**Requirements**: MinGW GCC (Dev-C++) or any C++11-compatible g++

```bash
# Full build
g++ -o WareHouse.exe main.cpp Menu.cpp WarehouseSystem.cpp \
    GoodsManager.cpp SqList.cpp BTree.cpp CategoryTree.cpp \
    MatGraph.cpp FileManager.cpp

# Run
./WareHouse.exe
```

Alternatively, open `MyWareHouse.dev` in Dev-C++ IDE.

## Menu

```
1.  Add Goods       6.  Display All       11. Shortest Path
2.  Delete Goods    7.  Browse by Category  0. Exit
3.  Update Goods    8.  Category Statistics
4.  Query by ID     9.  Load Demo Data
5.  Query by ID    10.  Save to File
```

## License

[MIT](LICENSE) © 2026 Githubzline
