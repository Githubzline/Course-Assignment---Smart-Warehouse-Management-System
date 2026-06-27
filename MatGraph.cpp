#include <iostream>
#include <string.h>
#include "MatGraph.h"

void CreateGraph(MatGraph &g,int A[][MAXVEX],int n,int e)
{  int i,j;
   g.n=n; g.e=e;
   for (i=0;i<n;i++)
      for (j=0;j<n;j++)
        g.edges[i][j]=A[i][j];
}
void DestroyGraph(MatGraph g)
{  }

void DispGraph(MatGraph g)
{  int i,j;
   // 列标题
   printf("%6s","");
   for (j=0;j<g.n;j++)
       printf("%4s", g.vexs[j].data);
   printf("\n");
   // 矩阵行（带行标题）
   for (i=0;i<g.n;i++)
   {  printf("%4s [", g.vexs[i].data);
      for (j=0;j<g.n;j++)
      {  if (g.edges[i][j]<INF)
           printf("%4d",g.edges[i][j]);
         else
           printf("%4s","--");
      }
      printf(" ]\n");
   }
}

void InitMatGraph(MatGraph &g)
{
    g.n = 0;
    g.e = 0;
    for (int i = 0; i < MAXVEX; i++)
        for (int j = 0; j < MAXVEX; j++)
            g.edges[i][j] = (i == j) ? 0 : INF;
}

/*
 * InitDemoGraph -- 初始化 2x5 仓库布局 Demo
 *
 * 仓库布局：
 *   A01---5m---A02---5m---A03---5m---A04---5m---A05
 *    |          |          |          |          |
 *    8m         8m         8m         8m         8m
 *    |          |          |          |          |
 *   B01---5m---B02---5m---B03---5m---B04---5m---B05
 *
 * 水平通道 5m，垂直通道 8m，共 26 条边
 */
void InitDemoGraph(MatGraph &g)
{
    InitMatGraph(g);

    const char *names[] = {"A01","A02","A03","A04","A05",
                           "B01","B02","B03","B04","B05"};
    int n = 10;
    g.n = n;

    for (int i = 0; i < n; i++)
        strcpy(g.vexs[i].data, names[i]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            g.edges[i][j] = (i == j) ? 0 : INF;

    // 水平通道：A排 5m
    for (int i = 0; i < 4; i++)
    {
        g.edges[i][i+1] = 5;
        g.edges[i+1][i] = 5;
    }
    // 水平通道：B排 5m
    for (int i = 0; i < 4; i++)
    {
        g.edges[5+i][5+i+1] = 5;
        g.edges[5+i+1][5+i] = 5;
    }
    // 垂直通道：8m
    for (int i = 0; i < 5; i++)
    {
        g.edges[i][5+i] = 8;
        g.edges[5+i][i] = 8;
    }

    g.e = 26;
}

/*
 * FindLocation -- 根据位置名查找顶点下标
 * 返回下标（0-based），未找到返回 -1
 */
int FindLocation(MatGraph &g, const char *loc)
{
    for (int i = 0; i < g.n; i++)
    {
        if (strcmp(g.vexs[i].data, loc) == 0)
            return i;
    }
    return -1;
}

int Degree1(MatGraph g,int v)  // 求无向图中顶点的度
{  int i,d=0;
   if (v<0 || v>=g.n)
      return -1;   // 顶点号错误返回-1
   for (i=0;i<g.n;i++)
   {  if (g.edges[v][i]>0 && g.edges[v][i]<INF)
      d++;  // 统计第v行既不为0也不为∞的边数
   }
   return d;
}

int Degree2(MatGraph g,int v)  // 求有向图中顶点的度
{  int i,d1=0,d2=0,d;
   if (v<0 || v>=g.n)
      return -1; // 顶点号错误返回-1
   for (i=0;i<g.n;i++)
   {  if (g.edges[v][i]>0 && g.edges[v][i]<INF)
      d1++;  // 统计第v行既不为0也不为∞的出边数
   }
   for (i=0;i<g.n;i++)
   {  if (g.edges[i][v]>0 && g.edges[i][v]<INF)
      d2++;  // 统计第v列既不为0也不为∞的入边数
   }
   d=d1+d2;
   return d;
}

void Dijkstra(MatGraph g,int v)
// 求顶点v到其余顶点的最短路径
{  int dist[MAXVEX];   // 定义dist数组
   int path[MAXVEX];   // 定义path数组
   int S[MAXVEX];      // 定义S数组
   int mindis,i,j,u=0;
   for (i=0;i<g.n;i++)
   {  dist[i]=g.edges[v][i]; // 距离初始化
      S[i]=0;                // S[]置空
      if (g.edges[v][i]<INF) // 路径初始化
        path[i]=v;           // v到i有边时，置i前一个顶点为v
      else                   // v到i没边时，置i前一个顶点为-1
        path[i]=-1;
   }
   S[v]=1;                   // 源点编号v放入S中
   for (i=0;i<g.n-1;i++)     // 循环向S中添加n-1个顶点
   {  mindis=INF;            // mindis置最小长度初值
      for (j=0;j<g.n;j++)    // 选取不在S中且有最小距离的顶点u
      {  if (S[j]==0 && dist[j]<mindis)
         {  u=j;
            mindis=dist[j];
         }
      }
      S[u]=1;                // 顶点u加入S中
      for (j=0;j<g.n;j++)    // 修改不在s中的顶点的距离
      {  if (S[j]==0)
         {  if (g.edges[u][j]<INF
                 && dist[u]+g.edges[u][j]<dist[j])
            {  dist[j]=dist[u]+g.edges[u][j];
               path[j]=u;
            }
         }
      }
   }
}

void Floyd(MatGraph g)    // 求每对顶点之间的最短路径
{  int A[MAXVEX][MAXVEX];     // 定义A数组
   int path[MAXVEX][MAXVEX];  // 定义path数组
   int i,j,k;
   for (i=0;i<g.n;i++)        // 给数组A和path置初值
   {  for (j=0;j<g.n;j++)
      {  A[i][j]=g.edges[i][j];
         if (i!=j && g.edges[i][j]<INF)
            path[i][j]=i;     // i和j顶点之间有边时
         else                 // i和j顶点之间没有边时
            path[i][j]=-1;
      }
   }
   for (k=0;k<g.n;k++)                    // 求Ak[i][j]
   {  for (i=0;i<g.n;i++)
      {  for (j=0;j<g.n;j++)
         {  if (A[i][j]>A[i][k]+A[k][j])  // 找到更短路径
            {  A[i][j]=A[i][k]+A[k][j];   // 修改路径长度
               path[i][j]=path[k][j];     // 修改最短路径
            }
         }
      }
   }
}
