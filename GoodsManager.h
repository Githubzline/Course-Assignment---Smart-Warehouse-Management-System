#ifndef GOODSMANAGER_H
#define GOODSMANAGER_H

#include "SqList.h"
#include "Goods.h"

/*
 * ============================================================
 * GoodsManager.h —— 货物 CRUD 封装层
 * ============================================================
 *
 * 把 SqList 的"按位置操作"包装成"按 ID 操作"，
 * 屏蔽底层顺序表的细节，方便 Menu 层调用。
 *
 * 依赖：SqList（InitList, GetElem, InsElem, DelElem, GetLength）
 *       Goods（结构体定义 MAX_NAME, MAX_LOC, MAX_DATE）
 * ============================================================
 */

// 初始化
void GM_Init(SqList &L);

// 按位置获取货物（1-based，与 SqList 一致）
int  GM_GetIndex(SqList &L, int i, Goods &g);

// 按编号（ID）查询货物 —— 最常用的查询方式
// 返回值：1=找到，g 存放结果；0=未找到
int  GM_GetId(SqList &L, int id, Goods &g);

// 按编号查找在顺序表中的位置（1-based 编号）
// 返回值：序号（>=1）；0=未找到
int  GM_FindPos(SqList &L, int id);

// 入库（添加货物）：末尾追加，自动检查 ID 是否重复
// 返回值：1=成功；0=失败（ID 重复或表满）
int  GM_Add(SqList &L, Goods g);

// 出库（删除货物）：按 ID 删除
// 返回值：1=成功；0=失败（未找到）
int  GM_Delete(SqList &L, int id);

// 修改货物信息：按 ID 查找
// 注意：如果 ID 也改了，检查新 ID 是否和现有货物冲突
// 返回值：1=成功；0=失败（未找到或 ID 冲突）
int  GM_Update(SqList &L, int id, Goods g);

// 返回当前货物总数
int  GM_Count(SqList &L);

// 打印全部货物列表（表格形式）
void GM_DisplayAll(SqList &L);

#endif
