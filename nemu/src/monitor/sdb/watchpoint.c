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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint { //链表的结构，指向下一个监视点
  int NO; //监视点的序号
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  int old_value; //旧值
  int new_value; //新值
  char *expr; //监视点名字
  //最后使用log函数，得到监视点在哪个地方发生改变

} WP;

static WP wp_pool[NR_WP] = {}; //创建一个池，用来储存数据
static WP *head = NULL, *free_ = NULL; //head组织使用中的监视点结构，free_用于组织空闲的监视点结构

void init_wp_pool() { //初始化池的内容
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }
  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

