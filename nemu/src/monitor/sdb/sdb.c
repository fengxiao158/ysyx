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

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <memory/paddr.h>
#include <debug.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1); //传入-1,但是因为他的参数是无符号类型的，这样会使得传入的数非常大，64位全为1
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
  char *arg = strtok(NULL, " ");
  cpu_exec(*arg);

  return 0;
}

static int cmd_info(char *args){
  char *arg = strtok(NULL, " ");
  if (*arg=='r')
  {
    isa_reg_display();
  }
  else if (*arg=='w')
  {

  }

  return 0;
}

static int cmd_x(char *args){
  char *arg1 = strtok(NULL, " ");
  char *arg2 = strtok(NULL, " ");
  int n=strtol(arg1,NULL,10);
  paddr_t expr=strtol(arg2,NULL,16);
  
  paddr_t value;

  for (int i=0;i<n;i++){
    value=paddr_read(expr+i,4); //打印pmem_read的值
    printf("%x ",value);
  }

  printf("\n");
  return 0;
}

static int cmd_p(char *args){
  bool *state=malloc(sizeof(bool));
  int value=expr(args,state);
  if (*state==true)
  {
    printf("value=%d\n",value);
    free(state);
    return true;
  }
  else 
    panic("cmd_p error!");
  free(state);
  return false;
}

static int cmd_w(char *args){
  return 0;
}

static int cmd_d(char *args){
  return 0;
}

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  {"si","Let the program N instructions step by step before pausing execution.WhenNis not ptovided,the default is 1,",cmd_si},
  {"info","Print register status or print watchpoint information.",cmd_info},
  {"x","Evaluate the expression EXPR to obtain its value,then output N consecutive 4-byte memory addresses starting from the result in hexadecimal format.",cmd_x},
  {"p","Evaluate the expression EXPR to obtain its value",cmd_p},
  {"w","Pause program execution when value of the expression EXPR changes",cmd_w},
  {"d","Delete the watchpoint with index N.",cmd_d}
};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");  //每调用一次，就分割一次
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL); //输入c之后，客户程序会执行，当客户程序达到要求的循环次数，或者执行了nemu_trap指令，才会结束
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " "); //字符串分割函数，将str里面的字符串按照设定的" "分割
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void test_expr()
{
  FILE *fp=fopen("/home/xiaofeng/ysyx-workbench/nemu/tools/gen-expr/input","r");
  if (fp==NULL) panic("test_expr error1!");

  char *expression=NULL; //存储从文件中读取的表达式
  word_t correct_value; //用于存储从文件中读取的预期结果
  size_t len=0; //读取行的长度
  __ssize_t read; //读取的字符数
  bool *success=malloc(sizeof(bool));
  word_t expr_value;
  while (1){
    if (fscanf(fp,"%u",&correct_value)==-1) break;
    read=getline(&expression,&len,fp);
    expression[read-1]='\0';
    expr_value=expr(expression,success);
    // printf("%s\n",expression);
    // if (*success!=true) panic("test_expr error2!");
    if (correct_value!=expr_value){
      printf("%s\n",expression);
      printf("expected: %u,got: %u\n",correct_value,expr_value);
      // panic("test_expr error3!");
    }
  }
  fclose(fp);
  free(success);
  panic("test_expr seccess!");
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  // test_expr();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
