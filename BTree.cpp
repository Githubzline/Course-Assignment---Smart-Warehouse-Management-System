#include <iostream>
#include <cstdlib>
#include "BTree.h"
void CreateBTree(BTNode * &bt,char *str)
{  BTNode *St[MAX_SIZE],*p=NULL;
   int top=-1,k,j=0;
   char ch;
   bt=NULL;  //建立的二叉树初始时为空
   ch=str[j];
 while (ch!='\0')       //str未扫描完时循环
   { switch(ch)
 {
 case '(':top++;St[top]=p;k=1; break;    //为左孩子结点
 case ')':top--;break;
 case ',':k=2; break;       //为右孩子结点
 default:p=(BTNode *)malloc(sizeof(BTNode));
  p->data=ch;p->lchild=p->rchild=NULL;
  if (bt==NULL)       //p为二叉树的根结点
    bt=p;
  else        //已建立二叉树的根结点
  {   switch(k)
    {
    case 1:St[top]->lchild=p;break;
    case 2:St[top]->rchild=p;break;
    }
  }
      }
      j++; ch=str[j];
   }
}
void DestroyBTree(BTNode *&bt)
{   if (bt!=NULL)
  { DestroyBTree(bt->lchild);
 DestroyBTree(bt->rchild);
       free(bt);
  }
}
int BTHeight(BTNode *bt)
{  int lchilddep,rchilddep;
   if (bt==NULL) return(0);      //空树的高度为0
   else
   {  lchilddep=BTHeight(bt->lchild);  //求左子树的高度
      rchilddep=BTHeight(bt->rchild);  //求右子树的高度
      return (lchilddep>rchilddep)?
                  (lchilddep+1):(rchilddep+1);
   }
}
int NodeCount(BTNode *bt)  //求二叉树bt的结点个数
{  int num1,num2;
   if (bt==NULL)   //为空树时返回0
       return 0;
   else
   {   num1=NodeCount(bt->lchild); //求左子树的结点数
       num2=NodeCount(bt->rchild); //求右子树的结点数
       return (num1+num2+1);  //返回和加上1
   }
}
int LeafCount(BTNode *bt) //求二叉树bt的叶子结点个数
{  int num1,num2;
   if (bt==NULL)  //空树返回0
 return 0;
   else if (bt->lchild==NULL && bt->rchild==NULL)
 return 1;  //为叶子结点时返回1
   else
   { num1=LeafCount(bt->lchild); //求左子树的叶子结点数
 num2=LeafCount(bt->rchild);  //求右子树的叶子结点数
 return (num1+num2);  //返回和
   }
}
void DispBTree(BTNode *bt)
{  if (bt!=NULL)
   {  printf("%d",bt->data);
      if (bt->lchild!=NULL || bt->rchild!=NULL)
      {  printf("(");   //有孩子时输出'('
  DispBTree(bt->lchild); //递归处理左子树
  if (bt->rchild!=NULL) //有右孩子时输出','
      printf(",");
  DispBTree(bt->rchild); //递归处理右子树
  printf(")");   //最后输出一个')'
      }
   }
}
void trans1(BTNode *bt, SqBinTree &sb,int i)
{ //i的初值为根结点编号1
   if (bt!=NULL)
   {  sb[i]=bt->data;   //将结点数据放入sb中
      trans1(bt->lchild,sb,2*i); //递归建立左子树
      trans1(bt->rchild,sb,2*i+1); //递归建立右子树
   }
   else sb[i]='#';  //对于不存在的结点对应位置值为'#'
}
void trans2(BTNode *&bt,SqBinTree sb,int i)
{  //i的初值为根结点编号1
  if (i<MAX_SIZE && sb[i]!='#') //当i有效且不为空时
  { bt=(BTNode *)malloc(sizeof(BTNode));  //创建根结点
 bt->data=sb[i];
 trans2(bt->lchild,sb,2*i); //递归建立左子树
 trans2(bt->rchild,sb,2*i+1); //递归建立右子树
   }
   else bt=NULL;  //无效结点对应的二叉树为NULL
}
void PreOrder(BTNode *bt)
{
 if(bt!=NULL)
 {
  printf("%d ",bt->data);
  PreOrder(bt->lchild);
  PreOrder(bt->rchild);
 }
}
void InOrder(BTNode *bt)
{
 if(bt!=NULL)
 {
  InOrder(bt->lchild);
  printf("%d ",bt->data);
  InOrder(bt->rchild);
 }
}
void PostOrder(BTNode *bt)
{
 if(bt!=NULL)
 {
  PostOrder(bt->lchild);
  PostOrder(bt->rchild);
  printf("%d ",bt->data);
 }
}

void LevelOrder(BTNode *bt)
{
 BTNode *p;
   BTNode *qu[MAX_SIZE];
  int front,rear;
  front=rear=0;
  rear++; qu[rear]=bt;
  while (front!=rear)
  {
    front=(front+1)%MAX_SIZE;
  p=qu[front];
  printf("%d ",p->data);
  if (p->lchild!=NULL)
  {
   rear=(rear+1)%MAX_SIZE;
     qu[rear]=p->lchild;
  }
  if (p->rchild!=NULL)
  {
   rear=(rear+1)%MAX_SIZE;
     qu[rear]=p->rchild;
  }
   }
}


