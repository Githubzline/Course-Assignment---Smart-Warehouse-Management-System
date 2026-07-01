#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "WarehouseSystem.h"

/*
 * ============================================================
 * FileManager.h —— 文件持久化
 * ============================================================
 *
 * 文件格式：纯文本，竖线分隔
 *   1001|螺丝刀|五金|A01|50|2026-06-01
 *   1002|电钻|五金|A02|30|2026-06-02
 *
 * 保存：遍历顺序表，逐行写入
 * 加载：逐行读取，解析后走 WS_AddGoods
 * ============================================================
 */

// 保存全部货物到 warehouse.dat
void FM_Save(WarehouseSystem &ws);

// 从 warehouse.dat 加载货物
void FM_Load(WarehouseSystem &ws);

#endif
