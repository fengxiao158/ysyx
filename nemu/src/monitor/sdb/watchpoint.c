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

void *return_head(){
  return head;
}

void *return_free(){
  return free;
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
    head=NULL;
  }
  else {
    pre=head;
    while (pre!=wp){
      pre=pre->next;
    }
    pre->next=wp->next;
  }
  wp->next=NULL;
  wp_num--;
}

void wp_add(char *wp_expr){
  WP *wp=new_wp();
  bool *success=malloc(sizeof(bool));
  wp->expr=wp_expr;
  // wp->old_value=expr(wp_expr,success); //程序里面才会有变量，但我不知道这个程序变量要怎么弄
  printf("Watchpoint %d: %s\n",wp->NO,wp_expr);
  free(success);
}

void wp_remove(char *wp_expr){
  WP *wp=head;
  while (wp->expr!=wp_expr){
    wp=wp->next;
    if (wp->next==NULL){
      panic("delete watchpoint error!");
    }
  }
  printf("Delete watchpoint %d: %s\n",wp->NO,wp->expr);
  free_wp(wp);
}

void wp_scan(){
  WP *wp=head;
  if (wp==NULL){
    printf("No watchpoints");
    return ;
  }
  while (wp!=NULL){
    printf("watchpoint%d : %s\n",wp->NO,wp->expr);
    wp=wp->next;
  }
}

void wp_compare(){
  WP *wp=head;
  bool *success=malloc(sizeof(success));
  while (wp!=NULL){
    word_t new=expr(wp->expr,success);
    if (wp->new_value!=new){
      printf("watchpoint %d:%s\n",wp->NO,wp->expr);
      printf("old value:%d",wp->old_value);
      printf("new value:%d",wp->new_value);
      wp->old_value=new;
    }
    wp=wp->next;
  }
}
