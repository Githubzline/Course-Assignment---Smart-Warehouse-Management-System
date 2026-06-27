#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "FileManager.h"

/*
 * 文件格式说明：
 *   每行一条货物，字段用 '|' 分隔
 *   格式：编号|名称|类别|货位|数量|入库日期
 */

// ========== FM_Save ==========
void FM_Save(WarehouseSystem &ws)
{
    FILE *fp = fopen(DATA_FILE, "w");    // 用 Common.h 里定义的 DATA_FILE
    if (fp == NULL) {
        printf("警告：无法打开文件 %s 进行保存。\n", DATA_FILE);
        return;
    }

    SqList &L = ws.goodsList;
    for (int i = 0; i < L.length; i++) {
        Goods *g = &L.data[i];
        fprintf(fp, "%d|%s|%s|%s|%d|%s\n",
                g->id, g->name, g->category,
                g->location, g->quantity, g->entryDate);
    }

    fclose(fp);
    printf("已保存 %d 条货物到 %s。\n", L.length, DATA_FILE);
}

// ========== FM_Load ==========
void FM_Load(WarehouseSystem &ws)
{
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        printf("未找到数据文件 %s，启动空白仓库。\n", DATA_FILE);
        return;
    }

    char buf[MAX_LINE];
    int count = 0;
    while (fgets(buf, MAX_LINE, fp) != NULL) {
        // 去掉末尾换行
        buf[strcspn(buf, "\r\n")] = '\0';
        if (buf[0] == '\0') continue;

        Goods g;
        // 按 | 解析：编号|名称|类别|货位|数量|日期
        sscanf(buf, "%d|%[^|]|%[^|]|%[^|]|%d|%[^|]",
               &g.id, g.name, g.category, g.location,
               &g.quantity, g.entryDate);

        WS_AddGoods(ws, g);
        count++;
    }

    fclose(fp);
    printf("已从 %s 加载 %d 条货物。\n", DATA_FILE, count);
}
