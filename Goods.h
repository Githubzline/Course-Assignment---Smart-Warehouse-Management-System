#ifndef Goods_H
#define Goods_H

#include "Common.h"
typedef struct Goods {
    // 货物名称，如 "螺丝刀"、"矿泉水"、"打印纸"
    char name[MAX_NAME];

    // 货物编号 — 这是货物的唯一标识（类似身份证号）
    // 程序中所有查找、删除、修改操作都靠这个 ID 来定位货物
    int  id;

    // 所属类别，如 "五金"、"电子"、"食品"、"办公用品"
    // 类别会用于分类浏览和统计功能
    char category[MAX_NAME];

    // 存储货位编号，如 "A01"、"B03"
    // 对应仓库布局图中的具体位置，用于最短路径计算
    char location[MAX_LOC];

    // 当前库存数量（件/个/箱）
    int  quantity;

    // 入库日期，固定格式 YYYY-MM-DD（如 "2026-06-01"）
    char entryDate[MAX_DATE];
} Goods;








#endif
