#ifndef SqList_H
#define SqList_H


#include "Goods.h"
#include "Common.h"
typedef Goods SqElemType;   
typedef struct
{
  SqElemType data[MAX_SIZE];   
 int length;   
}SqList;       

void InitList(SqList &L);
void DestroyList(SqList L);
int GetLength(SqList L);
int GetElem(SqList L,int i,SqElemType &e);
int Locate(SqList L,SqElemType x);
int InsElem(SqList &L,SqElemType x,int i);
int DelElem(SqList &L,int i);
void DispList(SqList L);

#endif
