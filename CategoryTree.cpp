#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "CategoryTree.h"

/*
 * ============================================================
 * CategoryTree.cpp —— 类别 BST 索引实现
 * ============================================================
 *
 * PPT 对照表：
 *   PPT 函数              我们的函数          差异
 *   ──────────────────────────────────────────────────
 *   BSTSearch(bt,k)      同                  比较方式 strcmp
 *   BSTInsert(bt,k)      同，多了 idx 参数    key 改为 char[]
 *   DestroyBST(bt)       同                  不变
 *
 * 变量命名对齐 PPT：
 *   bt = 树根       k = 关键字       f = 双亲
 *   p = 遍历指针    cmp = 比较结果
 * ============================================================
 */

// ========== PPT slide 35: BSTSearch ==========
// 改编：k<p->key 改为 strcmp(k, p->key)
BSTNode* BSTSearch(BSTNode *bt, const char *k)
{
    BSTNode *p = bt;
    while (p != NULL) {
        int cmp = strcmp(k, p->key);
        if (cmp == 0)
            return p;                    // 找到，返回节点指针
        else if (cmp < 0)
            p = p->lchild;               // 在左子树中查找
        else
            p = p->rchild;               // 在右子树中查找
    }
    return NULL;                         // 未找到
}

// ========== PPT slide 38-39: BSTInsert ==========
// 改编：k<p->key 改为 strcmp，多了一个 idx 参数
int BSTInsert(BSTNode *&bt, const char *k, int idx)
{
    BSTNode *f, *p = bt;
    while (p != NULL) {                  // 找插入位置，f 指向双亲
        if (strcmp(k, p->key) == 0)
            return 0;                    // 不能插入相同关键字
        f = p;                           // f 指向 p 的双亲节点
        if (strcmp(k, p->key) < 0)
            p = p->lchild;               // 在左子树中查找
        else
            p = p->rchild;               // 在右子树中查找
    }
    p = (BSTNode *)malloc(sizeof(BSTNode));
    strcpy(p->key, k);                   // 创建一个存放关键字k的新节点
    p->idx = idx;
    p->lchild = p->rchild = NULL;        // 新节点均作为叶子节点插入
    if (bt == NULL)                      // 原树为空时，p作为根节点
        bt = p;
    else if (strcmp(k, f->key) < 0)
        f->lchild = p;                   // 插入p作为f的左孩子
    else
        f->rchild = p;                   // 插入p作为f的右孩子
    return 1;                            // 插入成功返回1
}

// ========== PPT slide 46: DestroyBST ==========
void DestroyBST(BSTNode *&bt)
{
    if (bt != NULL) {
        DestroyBST(bt->lchild);          // 销毁左子树
        DestroyBST(bt->rchild);          // 销毁右子树
        free(bt);                        // 释放根节点
    }
}

// ========== CT_Init ==========
void CT_Init(CategoryTree &ct)
{
    ct.root = NULL;
    ct.catCount = 0;
}

// ========== CT_FindCategory ==========
// 调用 BSTSearch 查找类别名，返回 cats[] 下标
int CT_FindCategory(CategoryTree &ct, const char *name)
{
    BSTNode *p = BSTSearch(ct.root, name);
    if (p != NULL)
        return p->idx;                   // 找到，返回下标
    return -1;                           // 未找到
}

// ========== CT_AddCategory ==========
// ① cats[] 末尾追加  ② 调用 BSTInsert
int CT_AddCategory(CategoryTree &ct, const char *name)
{
    // 先查重
    if (CT_FindCategory(ct, name) >= 0)
        return 0;

    // cats[] 末尾追加
    int idx = ct.catCount;
    strcpy(ct.cats[idx].name, name);
    ct.cats[idx].goodsCount = 0;
    ct.catCount++;

    // 调用 PPT 的 BSTInsert
    return BSTInsert(ct.root, name, idx);
}

// ========== CT_InsertGoods ==========
void CT_InsertGoods(CategoryTree &ct, int catIdx, int goodsId)
{
    if (catIdx < 0 || catIdx >= ct.catCount)
        return;
    int cnt = ct.cats[catIdx].goodsCount;
    ct.cats[catIdx].goodsIds[cnt] = goodsId;
    ct.cats[catIdx].goodsCount++;
}

// ========== CT_RemoveGoods ==========
int CT_RemoveGoods(CategoryTree &ct, int catIdx, int goodsId)
{
    if (catIdx < 0 || catIdx >= ct.catCount)
        return 0;
    int cnt = ct.cats[catIdx].goodsCount;
    for (int i = 0; i < cnt; i++) {
        if (ct.cats[catIdx].goodsIds[i] == goodsId) {
            // 后面的往前挪
            for (int j = i; j < cnt - 1; j++)
                ct.cats[catIdx].goodsIds[j] = ct.cats[catIdx].goodsIds[j+1];
            ct.cats[catIdx].goodsCount--;
            return 1;
        }
    }
    return 0;
}

// ========== CT_DisplayByCategory ==========
// 中序遍历 BST，按字典序列打印所有类别
static void InOrderDisp(BSTNode *p, CategoryTree &ct)
{
    if (p == NULL) return;
    InOrderDisp(p->lchild, ct);
    printf("  [%s] %d kinds\n", ct.cats[p->idx].name, ct.cats[p->idx].goodsCount);
    InOrderDisp(p->rchild, ct);
}

void CT_DisplayByCategory(CategoryTree &ct)
{
    printf("\n========== 按类别浏览货物 ==========\n");
    InOrderDisp(ct.root, ct);
}

// ========== CT_DisplayStats ==========
void CT_DisplayStats(CategoryTree &ct)
{
    printf("\n========== 类别统计 ==========\n");
    for (int i = 0; i < ct.catCount; i++) {
        printf("  %-20s: %d kinds\n", ct.cats[i].name, ct.cats[i].goodsCount);
    }
    printf("\n");
}
