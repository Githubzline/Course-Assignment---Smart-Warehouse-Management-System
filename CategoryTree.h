#ifndef CATEGORYTREE_H
#define CATEGORYTREE_H

#include "Common.h"
#include "Goods.h"
#include "SqList.h"
/*
 * ============================================================
 * CategoryTree.h — 类别二叉树索引（PPT 第8章 BST 设计）
 * ============================================================
 *
 * 设计思·： BST（二叉排序树）+ 旁路数组
 *   BST 节点存类别名(key) + cats[] 下标(idx)，按类别名字典序排列
 *   cats[] 是旁路数组，存类别名 + 该类下所有货物 ID
 *
 * 与 PPT 的差异：
 *   1. PPT BSTNode.key 是 KeyType（整数/字符串），我们是 char[]
 *      比较用 strcmp 而不是 <
 *   2. PPT BSTNode.data 是 ElemType，我们 int idx（cats 下标）
 *   3. 比 PPT 多了 cats[] 旁路数组 + 货物 ID 列表
 * ============================================================
 */

// ---------- BST 节点（对标 PPT 8.3.1 slide 32，做了简化）----------
typedef struct BSTNode {
    char        key[MAX_NAME];    // 类别名（PPT 的 key，我们换成字符串）
    int         idx;              // cats[] 下标（PPT 的 data，我们换成下标）
    struct BSTNode *lchild, *rchild;
} BSTNode;

// ---------- 旁路数组元素 ----------
typedef struct {
    char name[MAX_NAME];                   // 类别名
    int  goodsIds[MAX_GOODS_PER_CAT];      // 该类别下货物 ID 列表
    int  goodsCount;                       // 货物种类数
} CategoryNode;

// ---------- CategoryTree ----------
typedef struct {
    BSTNode       *root;          // BST 根节点
    CategoryNode   cats[MAX_CATEGORY];  // 旁路数组
    int            catCount;      // 当前类别数
} CategoryTree;

// ====== 函数声明 ======

// 初始化 BST 根为 NULL，catCount=0

// ====== PPT 原版 BST 函数（对标 8.3.1 节）======

// 查找：对标 PPT slide 35 BSTSearch
BSTNode* BSTSearch(BSTNode *bt, const char *k);

// 插入：对标 PPT slide 38-39 BSTInsert
int BSTInsert(BSTNode *&bt, const char *k, int idx);

// 销毁：对标 PPT slide 46 DestroyBST
void DestroyBST(BSTNode *&bt);

void CT_Init(CategoryTree &ct);

// 按类别名在 cats[] 中查找，返回下标，未找到返回 -1
int  CT_FindCategory(CategoryTree &ct, const char *name);

// 添加新类别：① cats[] 末尾追加 ② BST 插入新节点（对标 PPT slide 39 BSTInsert）
// 成功返回 1，类别名已存在返回 0
int  CT_AddCategory(CategoryTree &ct, const char *name);

// 在指定类别下追加货物 ID
void CT_InsertGoods(CategoryTree &ct, int catIdx, int goodsId);

// 从指定类别下移除货物 ID（顺序查找 + 前移覆盖）
// 成功返回 1，未找到返回 0
int  CT_RemoveGoods(CategoryTree &ct, int catIdx, int goodsId);

// 中序遍历 BST，按字典序列所有类别（需要一个 SqList 来查货物详情）
void CT_DisplayByCategory(CategoryTree &ct, SqList &goodsList);

// 各类别统计：种类数 + 总库存
void CT_DisplayStats(CategoryTree &ct); 


#endif
