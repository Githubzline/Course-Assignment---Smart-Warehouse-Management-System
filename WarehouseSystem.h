#ifndef WAREHOUSESYSTEM_H
#define WAREHOUSESYSTEM_H

#include "GoodsManager.h"
#include "CategoryTree.h"
#include "MatGraph.h"

/*
 * ============================================================
 * WarehouseSystem.h —— 仓库系统服务层（协调层）
 * ============================================================
 *
 * 持有三个数据结构，负责跨结构操作：
 *   goodsList（顺序表） + catTree（类别树） + layout（仓库布局图）
 *
 * 关键规则：任何涉及多结构的操作，都必须通过 WS_ 函数完成，
 * 确保三处数据保持同步。
 * ============================================================
 */

typedef struct {
    SqList        goodsList;   // 顺序表：存储所有货物
    CategoryTree  catTree;     // BST + 旁路数组：类别索引
    MatGraph      layout;      // 邻接矩阵图：仓库货位布局
} WarehouseSystem;

// 初始化三个数据结构：清空数据 + 加载 Demo 地图
void WS_Init(WarehouseSystem &ws);

// 销毁所有动态内存
void WS_Destroy(WarehouseSystem &ws);

// 入库： 顺序表添加  类别树同步
// 返回 1=成功，0=失败（
int  WS_AddGoods(WarehouseSystem &ws, Goods g);

// 出库：类别树解关联 顺序表删除
// 返回 1=成功，0=失败
int  WS_DeleteGoods(WarehouseSystem &ws, int id);

// 修改货物：如果类别变了，先在旧类别树移除，再在新类别添加
// 返回 1=成功，0=失败
int  WS_UpdateGoods(WarehouseSystem &ws, int id, Goods gNew);

// 按 ID 查询并打印货物信息
void WS_QueryById(WarehouseSystem &ws, int id);

// 按类别浏览所有货物（中序遍历 BST）
void WS_BrowseByCategory(WarehouseSystem &ws);

// 各类别统计
void WS_ShowCategoryStats(WarehouseSystem &ws);

// 两货物间最短搬运路径（Dijkstra）
void WS_ShortestPath(WarehouseSystem &ws, int idA, int idB);

// 打印全部货物
void WS_DisplayAll(WarehouseSystem &ws);

// 加载 10 条 Demo 数据
void WS_LoadDemo(WarehouseSystem &ws);

// 保存到文件（委托 FileManager）
void WS_Save(WarehouseSystem &ws);

// 从文件加载（委托 FileManager）
void WS_Load(WarehouseSystem &ws);

#endif
