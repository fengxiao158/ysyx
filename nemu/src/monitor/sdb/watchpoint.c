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

static WP wp_pool[NR_WP] = {}; //创建一个池，用来储存数据，全局变量里面创建的变量，初始值为0
static WP *head = NULL, *free_ = NULL; //head组织使用中的监视点结构，free_用于组织空闲的监视点结构
int wp_num=0;

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

WP *new_wp(){
  WP *pre;
  pre=head;
  if (wp_num==0){ //第一个节点
    head=free_; 
    free_=free_->next;
    head->next=NULL;
    head->expr="expr";
    head->new_value=0;
    head->old_value=0;
    wp_num++;
    return head;
  }
  else if (wp_num!=0&&wp_num<32){
    while (pre->next!=NULL){
      pre=pre->next;
    }
    pre->next=free_;
    pre=free_;
    free_=free_->next;
    pre->next=NULL;
    pre->expr="expr";
    pre->new_value=0;
    pre->old_value=0;
    wp_num++;
    return pre;
  }
  else panic("too many watchpoint!");
}

void free_wp(WP *wp){
  WP *pre;
  pre=free_;
  while (pre->next!=NULL){
    pre=pre->next;
  }
  pre->next=wp;
  if (wp_num==1){ //当监视点只有一个的情况
    wp_num--;
    head=NULL;
  }
  else {
    pre=head;
    while (pre->next!=wp){
      pre=pre->next;
    }
    pre->next=wp->next;
  }
  wp->next=NULL;
  wp_num--;
}
