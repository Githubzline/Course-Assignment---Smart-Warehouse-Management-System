#include <cstring>
#include <cstdio>
#include "WarehouseSystem.h"
#include "FileManager.h"

/*
 * ============================================================
 * WarehouseSystem.cpp —— 服务层实现
 * ============================================================
 *
 * 本模块负责跨结构协调：
 *   - WS_AddGoods / WS_DeleteGoods / WS_UpdateGoods：保持三结构同步
 *   - WS_ShortestPath：查货位 → Dijkstra 算路径 → 打印结果
 * ============================================================
 */

// ========== WS_Init ==========
void WS_Init(WarehouseSystem &ws)
{
    GM_Init(ws.goodsList);       // 清空顺序表
    CT_Init(ws.catTree);         // 清空类别树
    InitDemoGraph(ws.layout);    // 加载 2×5 仓库 Demo 地图
}

// ========== WS_Destroy ==========
void WS_Destroy(WarehouseSystem &ws)
{
    DestroyBST(ws.catTree.root);   // 释放 BST 节点
    ws.catTree.root = NULL;
    ws.catTree.catCount = 0;
    // 顺序表不需要释放（定长数组）；图也无需释放
}

// ========== WS_AddGoods（入库：三处同步）==========
int WS_AddGoods(WarehouseSystem &ws, Goods g)
{
    // ① 顺序表中添加货物
    if (!GM_Add(ws.goodsList, g))
        return 0;                // ID 重复或表满

    // ② 类别树：没有这个类别就先建
    int idx = CT_FindCategory(ws.catTree, g.category);
    if (idx < 0) {
        CT_AddCategory(ws.catTree, g.category);
        idx = CT_FindCategory(ws.catTree, g.category);
    }

    // ③ 旁路数组：该类下追加这个货物 ID
    CT_InsertGoods(ws.catTree, idx, g.id);
    return 1;
}

// ========== WS_DeleteGoods（出库：先解关联再删）==========
int WS_DeleteGoods(WarehouseSystem &ws, int id)
{
    // ① 先查出货物（需要知道它属于哪个类别）
    Goods g;
    if (!GM_GetId(ws.goodsList, id, g))
        return 0;

    // ② 从类别树下移除该货物 ID
    int idx = CT_FindCategory(ws.catTree, g.category);
    CT_RemoveGoods(ws.catTree, idx, id);

    // ③ 从顺序表删除
    GM_Delete(ws.goodsList, id);
    return 1;
}

// ========== WS_UpdateGoods（修改：类别变了要迁移）==========
int WS_UpdateGoods(WarehouseSystem &ws, int id, Goods gNew)
{
    // 查出旧货物
    Goods gOld;
    if (!GM_GetId(ws.goodsList, id, gOld))
        return 0;

    // 如果类别变了：旧类别移除 + 新类别添加
    if (strcmp(gOld.category, gNew.category) != 0) {
        int oldIdx = CT_FindCategory(ws.catTree, gOld.category);
        CT_RemoveGoods(ws.catTree, oldIdx, id);

        int newIdx = CT_FindCategory(ws.catTree, gNew.category);
        if (newIdx < 0) {
            CT_AddCategory(ws.catTree, gNew.category);
            newIdx = CT_FindCategory(ws.catTree, gNew.category);
        }
        CT_InsertGoods(ws.catTree, newIdx, id);
    }

    // 更新顺序表中的数据
    return GM_Update(ws.goodsList, id, gNew);
}

// ========== WS_QueryById ==========
void WS_QueryById(WarehouseSystem &ws, int id)
{
    Goods g;
    if (GM_GetId(ws.goodsList, id, g)) {
        printf("\n货物详情：\n");
        printf("  编号：%d\n", g.id);
        printf("  名称：%s\n", g.name);
        printf("  类别：%s\n", g.category);
        printf("  货位：%s\n", g.location);
        printf("  数量：%d\n", g.quantity);
        printf("  日期：%s\n\n", g.entryDate);
    } else {
        printf("\n未找到编号为 %d 的货物。\n\n", id);
    }
}

// ========== WS_BrowseByCategory ==========
void WS_BrowseByCategory(WarehouseSystem &ws)
{
    CT_DisplayByCategory(ws.catTree);
}

// ========== WS_ShowCategoryStats ==========
void WS_ShowCategoryStats(WarehouseSystem &ws)
{
    CT_DisplayStats(ws.catTree);
}

// ========== WS_DisplayAll ==========
void WS_DisplayAll(WarehouseSystem &ws)
{
    GM_DisplayAll(ws.goodsList);
}

// ========== WS_ShortestPath ==========
// ① 查两货物的货位 ② Dijkstra ③ 回溯输出路径
void WS_ShortestPath(WarehouseSystem &ws, int idA, int idB)
{
    Goods gA, gB;
    if (!GM_GetId(ws.goodsList, idA, gA)) {
        printf("\n货物 %d 不存在。\n", idA);
        return;
    }
    if (!GM_GetId(ws.goodsList, idB, gB)) {
        printf("\n货物 %d 不存在。\n", idB);
        return;
    }

    // 货位名 → 顶点下标
    int vA = FindLocation(ws.layout, gA.location);
    int vB = FindLocation(ws.layout, gB.location);
    if (vA < 0 || vB < 0) {
        printf("\n货位不在仓库布局图中。\n");
        return;
    }

    printf("\n计算 %s(%s) → %s(%s) 的最短路径...\n",
           gA.name, gA.location, gB.name, gB.location);

    // ---- Dijkstra（直接在这算，方便输出）----
    MatGraph &g = ws.layout;
    #define INF 0x3f3f3f3f
    int dist[MAXVEX], path[MAXVEX], S[MAXVEX];

    // 初始化（PPT slide 56）
    for (int i = 0; i < g.n; i++) {
        dist[i] = g.edges[vA][i];
        S[i] = 0;
        if (g.edges[vA][i] < INF)
            path[i] = vA;
        else
            path[i] = -1;
    }
    S[vA] = 1;

    // 主循环（PPT slide 57）
    for (int i = 0; i < g.n - 1; i++) {
        int mindis = INF, u = -1;
        for (int j = 0; j < g.n; j++)
            if (S[j] == 0 && dist[j] < mindis) {
                u = j;
                mindis = dist[j];
            }
        if (u == -1) break;
        S[u] = 1;
        for (int j = 0; j < g.n; j++)
            if (S[j] == 0 && g.edges[u][j] < INF
                && dist[u] + g.edges[u][j] < dist[j]) {
                dist[j] = dist[u] + g.edges[u][j];
                path[j] = u;
            }
    }

    // 回溯路径
    if (dist[vB] >= INF) {
        printf("两货位之间没有连通路径。\n\n");
        return;
    }

    int route[MAXVEX], cnt = 0;
    int cur = vB;
    route[cnt++] = cur;
    while (cur != vA) {
        cur = path[cur];
        route[cnt++] = cur;
    }

    printf("最短路径：");
    for (int i = cnt - 1; i >= 0; i--) {
        printf("%s", g.vexs[route[i]].data);
        if (i > 0) printf(" → ");
    }
    printf("\n总距离：%d m\n\n", dist[vB]);
}

// ========== WS_LoadDemo（10 条示例数据）==========
void WS_LoadDemo(WarehouseSystem &ws)
{
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

    for (int i = 0; i < 10; i++)
        WS_AddGoods(ws, demo[i]);

    printf("\n已加载 10 条 Demo 数据。\n");
}

// ========== WS_Save / WS_Load ==========
void WS_Save(WarehouseSystem &ws) {
    FM_Save(ws);
}
void WS_Load(WarehouseSystem &ws) {
    FM_Load(ws);
}
