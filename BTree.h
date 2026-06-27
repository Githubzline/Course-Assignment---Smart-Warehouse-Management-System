#ifndef BTree_H
#define BTree_H


#include "Common.h"
typedef int ElemType;
typedef ElemType SqBinTree[MAX_SIZE];
typedef struct tnode
{ ElemType data; //结点值
struct tnode *lchild,*rchild; //指针域
} BTNode; //二叉链结点类型

void CreateBTree(BTNode * &bt,char *str);
void DestroyBTree(BTNode *&bt);
int BTHeight(BTNode *bt);
int NodeCount(BTNode *bt);  //求二叉树bt的结点个数
int LeafCount(BTNode *bt); //求二叉树bt的叶子结点个数
void DispBTree(BTNode *bt);
void trans1(BTNode *bt, SqBinTree &sb,int i);
void trans2(BTNode *&bt,SqBinTree sb,int i);
void PreOrder(BTNode *bt);
void InOrder(BTNode *bt);
void PostOrder(BTNode *bt);
void LevelOrder(BTNode *bt);
//BTNode* CreateBTreeFromPI(ElemType *pre, ElemType *in, int n);
//void FindPathSum(BTNode *bt, int sum);












#endif
