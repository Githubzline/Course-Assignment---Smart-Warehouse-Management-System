#include <cstdio>
#include "GoodsManager.h"

/*
 * ============================================================
 * GoodsManager.cpp —— 货物 CRUD 实现
 * ============================================================
 */

// ========== GM_Init ==========
void GM_Init(SqList &L)
{
    InitList(L);
}

// ========== GM_GetIndex ==========
int GM_GetIndex(SqList &L, int i, Goods &g)
{
    return GetElem(L, i, g);
}

// ========== GM_GetId ==========
int GM_GetId(SqList &L, int id, Goods &g)
{
    int pos = GM_FindPos(L, id);     // 先找位置
    if (pos == 0) return 0;          // 未找到
    return GetElem(L, pos, g);       // 获取元素
}

// ========== GM_FindPos ==========
int GM_FindPos(SqList &L, int id)
{
    for (int i = 0; i < L.length; i++)
    {
        if (L.data[i].id == id)
            return i + 1;            // 物理下标 -> 逻辑序号（1-based）
    }
    return 0;                        // 没找到
}

// ========== GM_Add ==========
int GM_Add(SqList &L, Goods g)
{
    // 检查 ID 是否已存在（去重）
    if (GM_FindPos(L, g.id) != 0)
        return 0;                    // ID 重复，拒绝入库

    // 在末尾追加：L.length + 1 表示最后一个位置之后
    return InsElem(L, g, L.length + 1);
}

// ========== GM_Delete ==========
int GM_Delete(SqList &L, int id)
{
    int pos = GM_FindPos(L, id);
    if (pos == 0) return 0;          // 没找到，错误返回
    return DelElem(L, pos);
}

// ========== GM_Update ==========
int GM_Update(SqList &L, int id, Goods g)
{
    int pos = GM_FindPos(L, id);
    if (pos == 0) return 0;          // 没找到原货物

    // 如果编号也改了 AND 新编号已被其他货物占用 -> 拒绝
    if (g.id != id && GM_FindPos(L, g.id) != 0)
        return 0;

    // 直接覆盖 data[pos-1]（物理下标）
    L.data[pos - 1] = g;
    return 1;
}

// ========== GM_Count ==========
int GM_Count(SqList &L)
{
    return GetLength(L);
}

// ========== GM_DisplayAll ==========
// 不调 SqList 的 DispList（它只认 int），而是自己按 Goods 字段排版
void GM_DisplayAll(SqList &L)
{
    if (L.length == 0)
    {
        printf("\n仓库为空，暂无货物。\n\n");
        return;
    }

    printf("\n| %-6s | %-12s | %-12s | %-6s | %-5s | %-11s |\n",
           "编号", "名称", "类别", "货位", "数量", "入库日期");
    printf("|--------|--------------|--------------|--------|-------|-------------|\n");

    for (int i = 0; i < L.length; i++)
    {
        Goods *g = &L.data[i];
        printf("| %-6d | %-12s | %-12s | %-6s | %-5d | %-11s |\n",
               g->id, g->name, g->category,
               g->location, g->quantity, g->entryDate);
    }
    printf("\n");
}
