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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUM

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces 有个加号，是为了能匹配多个空格
  {"\\+", '+'},         // plus "\\"是转义字符
  {"==", TK_EQ},        // equal
  {"-",'-'},
  {"\\*",'*'},
  {"/",'/'},
  {"[0-9]+", TK_NUM},
  {"\\(",'('},
  {"\\)",')'},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {}; //用于存储已编译的正则表达式模式的数组

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 *在简易调试器初始化的时候调用该初始化函数
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type; //type用于token类型
  char str[32]; //sre用于记录数字，比如123这样的十进制整数
} Token;

static Token tokens[32] __attribute__((used)) = {}; /*用于按顺序存放已经被识别出的token信息
后面的attribute用于告诉编译器这个这个东西可能没用，但依旧要编译，“={}”用于将数组初始化*/
static int nr_token __attribute__((used))  = 0;/*用于指示已经被识别出的token数目*/

/**
 * @brief 用于识别token
 * 
 * @param e 
 * @return true 
 * @return false 
 */
static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0; 

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         * 将识别出来的token记录下来，空格是例外，要单独处理
         */
        switch (rules[i].token_type) {
          case '+':
            tokens[nr_token].type='+'; 
            nr_token++;
            break;
          case '-':
            tokens[nr_token].type='-'; 
            nr_token++;
            break;
          case '*':
            tokens[nr_token].type='*'; 
            nr_token++;
            break;
          case '/':
            tokens[nr_token].type='/'; 
            nr_token++;
            break;
          case TK_NUM:
            strncpy(tokens[nr_token].str,substr_start,substr_len);  //将数字赋给str
            tokens[nr_token].str[substr_len]='\0'; //末尾加上空
            tokens[nr_token].type=TK_NUM;
            nr_token++;
            break;
          case '(':
            tokens[nr_token].type='('; 
            nr_token++;
            break;
          case ')':
            tokens[nr_token].type=')'; 
            nr_token++;
            break;
          case TK_NOTYPE:
            break;
          default: panic("tokens recognize wrong");
        }
        break;
      }
    }
    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  bool state;
  success=&state;
  if (make_token(e)==false) {
    state=false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();

  state=true;
  return 0;
}
