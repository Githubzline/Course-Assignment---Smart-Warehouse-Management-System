#ifndef MatGraph_H
#define MatGraph_H
#define MAXVEX 100     //图中最大顶点个数
#define INF 0x3f3f3f3f

typedef char VertexType[10];
                        //定义VertexType为字符串类型
typedef struct vertex
{  int adjvex;    //顶点编号
   VertexType data;    //顶点的信息
} VType;    //顶点类型
typedef struct graph
{  int n,e;    //n为实际顶点数,e为实际边数
   VType vexs[MAXVEX];  //顶点集合
   int edges[MAXVEX][MAXVEX];  //边的集合
}  MatGraph;    //图的邻接矩阵类型

void CreateGraph(MatGraph &g,int A[][MAXVEX],int n,int e);
void DestroyGraph(MatGraph g);
void DispGraph(MatGraph g);
void InitMatGraph(MatGraph &g);
void InitDemoGraph(MatGraph &g);
int FindLocation(MatGraph &g, const char *loc);
int Degree1(MatGraph g,int v);
int Degree2(MatGraph g,int v);
void Dijkstra(MatGraph g,int v);
void Floyd(MatGraph g);	//求每对顶点之间的最短路径



#endif
