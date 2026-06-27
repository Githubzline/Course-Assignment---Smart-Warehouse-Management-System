#include <cstdio>
#include <cstring>
#include "WarehouseSystem.h"
#include "FileManager.h"

/*
 * ============================================================
 * Menu.cpp —— 终端菜单（UI 层）
 * ============================================================
 *
 * 全局唯一 WarehouseSystem 实例，所有菜单操作围绕它展开。
 * 启动时自动加载文件 → 空仓库则载入 Demo → 循环菜单 → 退出保存。
 * ============================================================
 */

static WarehouseSystem ws;

// ========== 辅助：读整数 ==========
static int ReadInt()
{
    int val;
    scanf("%d", &val);
    while (getchar() != '\n');   // 清掉缓冲区残余
    return val;
}

// ========== 辅助：读字符串 ==========
static void ReadStr(char *buf, int maxlen)
{
    scanf("%s", buf);
    while (getchar() != '\n');
}

// ========== 打印框线菜单 ==========
static void ShowMenu()
{
    printf("\n");
    printf("  +------------------------------------------+\n");
    printf("  |        智能仓储管理系统 v1.0             |\n");
    printf("  +------------------------------------------+\n");
    printf("  |  1. 入库货物       6. 按类别浏览         |\n");
    printf("  |  2. 出库货物       7. 类别统计           |\n");
    printf("  |  3. 查询货物       8. 最短搬运路径       |\n");
    printf("  |  4. 修改货物       9. 加载 Demo 数据     |\n");
    printf("  |  5. 全部货物      10. 显示布局图         |\n");
    printf("  |                    0. 保存并退出         |\n");
    printf("  +------------------------------------------+\n");
    printf("  请选择: ");
}

// ========== 1. 入库 ==========
static void DoAdd()
{
    Goods g;
    printf("\n  >> 入库 <<\n");
    printf("  编号: "); g.id = ReadInt();
    printf("  名称: "); ReadStr(g.name, MAX_NAME);
    printf("  类别: "); ReadStr(g.category, MAX_NAME);
    printf("  货位: "); ReadStr(g.location, MAX_LOC);
    printf("  数量: "); g.quantity = ReadInt();
    printf("  入库日期(YYYY-MM-DD): "); ReadStr(g.entryDate, MAX_DATE);

    if (WS_AddGoods(ws, g))
        printf("\n  [OK] 货物 %s 入库成功。\n", g.name);
    else
        printf("\n  [FAIL] 入库失败：编号重复或仓库已满。\n");
}

// ========== 2. 出库 ==========
static void DoDelete()
{
    printf("\n  >> 出库 <<\n");
    printf("  要出库的货物编号: ");
    int id = ReadInt();

    if (WS_DeleteGoods(ws, id))
        printf("\n  [OK] 编号 %d 已出库。\n", id);
    else
        printf("\n  [FAIL] 未找到编号 %d。\n", id);
}

// ========== 3. 查询 ==========
static void DoQuery()
{
    printf("\n  >> 查询货物 <<\n");
    printf("  货物编号: ");
    int id = ReadInt();
    WS_QueryById(ws, id);
}

// ========== 4. 修改 ==========
static void DoUpdate()
{
    printf("\n  >> 修改货物 <<\n");
    printf("  要修改的货物编号: ");
    int id = ReadInt();

    Goods gNew;
    printf("  新名称: ");   ReadStr(gNew.name, MAX_NAME);
    printf("  新类别: ");   ReadStr(gNew.category, MAX_NAME);
    printf("  新货位: ");   ReadStr(gNew.location, MAX_LOC);
    printf("  新数量: ");   gNew.quantity = ReadInt();
    printf("  新日期: ");   ReadStr(gNew.entryDate, MAX_DATE);
    gNew.id = id;  // 默认不改编号

    if (WS_UpdateGoods(ws, id, gNew))
        printf("\n  [OK] 修改成功。\n");
    else
        printf("\n  [FAIL] 修改失败：货物不存在或编号冲突。\n");
}

// ========== 5. 全部货物 ==========
static void DoDisplayAll()
{
    WS_DisplayAll(ws);
}

// ========== 6. 按类别浏览 ==========
static void DoBrowseByCat()
{
    WS_BrowseByCategory(ws);
}

// ========== 7. 类别统计 ==========
static void DoCategoryStats()
{
    WS_ShowCategoryStats(ws);
}

// ========== 8. 最短路径 + ASCII 地图 ==========
static void DoShortestPath()
{
    printf("\n  >> 最短搬运路径 <<\n");
    printf("  货物 A 编号: "); int idA = ReadInt();
    printf("  货物 B 编号: "); int idB = ReadInt();

    WS_ShortestPath(ws, idA, idB);

    // 打印 ASCII 仓库地图
    printf("\n  仓库布局图:\n\n");
    MatGraph &g = ws.layout;
    if (g.n == 10) {
        // A 行
        printf("  A01---5m---A02---5m---A03---5m---A04---5m---A05\n");
        printf("   |          |          |          |          |\n");
        printf("   8m         8m         8m         8m         8m\n");
        printf("   |          |          |          |          |\n");
        printf("  B01---5m---B02---5m---B03---5m---B04---5m---B05\n");
        printf("\n  注: 水平通道 5m, 垂直通道 8m\n");
    }
}

// ========== 9. 加载 Demo ==========
static void DoLoadDemo()
{
    WS_LoadDemo(ws);
}

// ========== 10. 显示邻接矩阵 ==========
static void DoShowGraph()
{
    printf("\n  >> 仓库布局邻接矩阵 <<\n");
    printf("  (0=自己, INF=不连通, 数字=距离/米)\n\n");
    DispGraph(ws.layout);
}

// ========== 入口：Menu_Run ==========
void Menu_Run()
{
    printf("\n");
    printf("  ╔══════════════════════════════════════╗\n");
    printf("  ║   欢迎使用 智能仓储管理系统 v1.0    ║\n");
    printf("  ║   数据结构课程设计 — 综合项目       ║\n");
    printf("  ╚══════════════════════════════════════╝\n");

    WS_Init(ws);

    // 尝试加载文件，空仓库则自动载入 Demo
    FM_Load(ws);
    if (GM_Count(ws.goodsList) == 0) {
        printf("\n  仓库为空，自动加载 Demo 数据...\n");
        WS_LoadDemo(ws);
    }

    int choice;
    do {
        ShowMenu();
        choice = ReadInt();

        switch (choice) {
            case 1:  DoAdd();           break;
            case 2:  DoDelete();        break;
            case 3:  DoQuery();         break;
            case 4:  DoUpdate();        break;
            case 5:  DoDisplayAll();    break;
            case 6:  DoBrowseByCat();   break;
            case 7:  DoCategoryStats(); break;
            case 8:  DoShortestPath();  break;
            case 9:  DoLoadDemo();      break;
            case 10: DoShowGraph();     break;
            case 0:
                printf("\n  正在保存数据...\n");
                FM_Save(ws);
                printf("  谢谢使用，再见！\n\n");
                break;
            default:
                printf("\n  无效选择，请重新输入。\n");
        }
    } while (choice != 0);

    WS_Destroy(ws);
}
