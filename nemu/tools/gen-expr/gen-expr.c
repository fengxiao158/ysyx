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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

//这两个东西，是为了防止buf溢出
static char *buf_start=NULL; //指示buf开始的地方
static char *buf_end=buf+sizeof(buf); //指示buf结束的地方

int choose(int n){
  return rand()%n;
} 

/**
 * @brief 生成数字用的，其中的snprintf函数，是将某个类型的东西转换为字符串，第一个参数是字符串的首地址，
 * 第二个参数是字符串长度的限制
 * 
 */
static void gen_num(){
  int num=choose(INT8_MAX);
  if (buf_start<buf_end){
    int buffer_long=snprintf(buf_start,buf_end-buf_start,"%d",num); //末尾包含一个空字符串
    if (buffer_long>0){ //buffer_long是snprintf函数的返回值，返回的是当前的字符串长度
      buf_start+=buffer_long;
    }
  }
}

static void gen_char(char c){
  int buffer_long=snprintf(buf_start,buf_end-buf_start,"%c",c);
  if (buf_start<buf_end){
    if (buffer_long>0){
      buf_start+=buffer_long;
    }
  }
}

static char ops[]={'+','-','*','/'};
static void gen_rand_op(){
  int op_type=choose(sizeof(ops));
  char op=ops[op_type];
  gen_char(op);
}

static void gen_rand_expr() {
  switch (choose(3))
  {
  case 0:
    gen_num();
    break;
  case 1:
    gen_char('(');
    gen_rand_expr();
    gen_char(')');
    break;
  default: 
    gen_rand_expr();
    gen_rand_op();
    gen_rand_expr();
    break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf_start=buf;
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -Wall -Werror -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
