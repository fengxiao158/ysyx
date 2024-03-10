/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

typedef struct watchpoint { //链表的结构，指向下一个监视点
  int NO; //监视点的序号
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  int old_value; //旧值
  int new_value; //新值
  char *expr; //监视点名字
  //最后使用log函数，得到监视点在哪个地方发生改变
} WP;

word_t expr(char *e, bool *success);
WP *new_wp();
void free_wp(WP *wp);
void *return_head();
void *return_free();
void wp_add(char *wp_expr);
void wp_remove(char *wp_expr);
void wp_scan();
void wp_compare();

#endif
