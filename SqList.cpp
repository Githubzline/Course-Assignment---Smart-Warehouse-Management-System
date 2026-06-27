#include <iostream>
#include "SqList.h"


void InitList(SqList &L)
{
 L.length=0;
}

void DestroyList(SqList L)
{
 
}

int GetLength(SqList L)
{
 return L.length;
}

int GetElem(SqList L,int i,SqElemType &e)
{
 if (i<1||i>L.length)
  return 0;
 else
 {
  e=L.data[i-1];
  return 1; 
 }
}

int Locate(SqList L,SqElemType x)
{
 int i=0;
 while(i<L.length&&L.data[i].id!=x.id)
  i++;
 if (i>=L.length)
  return 0;
 else 
  return(i+1);
}

int InsElem(SqList &L,SqElemType x,int i)
{
 int j;
 if(i<1||i>L.length+1||L.length==MAX_SIZE)
  return 0;
 for (j=L.length;j>=i;j--)
  L.data[j]=L.data[j-1];
 L.data[i-1]=x;
 L.length++;
 return 1;
}

int DelElem(SqList &L,int i)
{
 int j;
 if(i<1||i>L.length)
  return 0; 
 for (j=i;j<L.length;j++)
  L.data[j-1]=L.data[j];
 L.length--;
 return 1;
}
void DispList(SqList L)
{
 int i;
 for (i=0;i<L.length;i++)
 {
  printf("%d %s %s %s %d %s ",L.data[i].id,  L.data[i].name,  L.data[i].category,  L.data[i].location,  L.data[i].quantity,  L.data[i].entryDate);
 }
 printf("\n");
 } 














