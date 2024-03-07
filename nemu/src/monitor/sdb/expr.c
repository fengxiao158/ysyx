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
#include <memory/paddr.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256,TK_NUM,TK_NEGNUM,TK_POSNUM,TK_POINT, //TK_DERED代表着负数,TK_POSNUM代表着正数，TK_POINT代表着解指针
  TK_NUMHEX, //16进制用的
  /* TODO: Add more token types */
  TK_EQ,TK_NEQ,TK_GT,TK_LT,TK_GE,TK_LE, //后四个分别是大于，小于，大于等于，小于等于
  TK_AND,TK_OR,
  TK_GPR, //表示寄存器

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
  {"0x[0-9a-fA-F]+", TK_NUMHEX},
  {"[0-9]+", TK_NUM},
  {"\\(",'('},
  {"\\)",')'},
  {"<=",TK_LE},
  {">=",TK_GE},
  {"<",TK_LT},
  {">",TK_GT},
  {"!=",TK_NEQ},
  {"&&",TK_AND},
  {"\\|\\|",TK_OR},
  {"\\$\\w+",TK_GPR}, //寄存器类型,输入$EXPR
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

static Token tokens[65536] __attribute__((used)) = {}; /*用于按顺序存放已经被识别出的token信息后面的attribute用于告诉编译器这个这个东西可能没用，但依旧要编译，“={}”用于将数组初始化，将本来的32改成可以防止内存违规访问*/
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
          case TK_NUMHEX:
            strncpy(tokens[nr_token].str,substr_start,substr_len);  //将数字赋给str
            tokens[nr_token].str[substr_len]='\0'; //末尾加上空
            tokens[nr_token].type=TK_NUMHEX;
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
          case TK_LT:
            tokens[nr_token].type=TK_LT;
            nr_token++;
            break;
          case TK_GT:
            tokens[nr_token].type=TK_GT;
            nr_token++;
            break;
          case TK_LE:
            tokens[nr_token].type=TK_LE;
            nr_token++;
            break;
          case TK_GE:
            tokens[nr_token].type=TK_GE;
            nr_token++;
            break;
          case TK_NEQ:
            tokens[nr_token].type=TK_NEQ;
            nr_token++;
            break;
          case TK_EQ:
            tokens[nr_token].type=TK_EQ;
            nr_token++;
            break;
          case TK_AND:
            tokens[nr_token].type=TK_AND;
            nr_token++;
            break;
          case TK_OR:
            tokens[nr_token].type=TK_OR;
            nr_token++;
            break;
          case TK_GPR:
            strncpy(tokens[nr_token].str,substr_start+1,substr_len);  //将数字赋给str
            tokens[nr_token].str[substr_len]='\0'; //末尾加上空
            tokens[nr_token].type=TK_GPR;
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

bool check_parentheses(int p, int q) {
  if (tokens[p].type=='(' && tokens[q].type==')') {
    int par = 0;
    for (int i = p; i <= q; i++) {
      if (tokens[i].type=='(') par++;
      else if (tokens[i].type==')') par--;
      if (par == 0) return i==q; //匹配到最左边的括号，括号匹配完毕
    }
  }
  return false;
}

int find_major(int p, int q,bool *success) {
  int ret = -1, par = 0, op_type = 0; /*ret是主运算符的索引，为返回值;op_type为找到的最高优先级算
  符；par是匹配的括号数量*/
  for (int i = p; i <= q; i++) {
    if (tokens[i].type == TK_NUM) {
      continue;
    }
    if (tokens[i].type == '(') {
      par++; //匹配到左括号，就让par++
    } 
    else if (tokens[i].type == ')') {
      if (par == 0) {
        *success=false; //匹配到右括号的时候，但是par却为0,代表这个式子有问题
        return 0;
      }
      par--; //匹配到右括号，让par--
    } 
    else if (par > 0) {
      continue; //在括号里面不可能是主运算符，所以continue
    } 
    else {
      int tmp_type = 0; //保存当前的运算符优先级
      switch (tokens[i].type) {
        case '*': case '/': tmp_type = 1; break;
        case '+': case '-': tmp_type = 2; break;
        case TK_AND: case TK_OR: tmp_type=3; break;
        case TK_GE: case TK_LE: case TK_LT: case TK_GT: case TK_EQ: case TK_NEQ: tmp_type=4; break;
        default: assert(0);
      }
      if (tmp_type >= op_type) {
        op_type = tmp_type; //如果最高的主运算符优先级低于当前的主运算符，则让最高的等于当前的
        ret = i; //记录下当前的主运算符位置
      }
    }
  }
  if (par != 0){
    *success=false;
    return 0;
  }
  return ret;
}

int eval(int p, int q, bool *state) {
  *state = true;
  if (p > q) {
    *state = false; //p>q的情况下，直接认为错误
    // panic("expr_test error2.2.1!");
    return 0;
  } 
  else if (p == q) {
    int ret;
    if (tokens[p].type==TK_NUM){
      ret = strtol(tokens[p].str, NULL, 10); //将字符串内的数字转为10进制的数据
      if (tokens[p-1].type==TK_NEGNUM) ret=-ret;
      else if (tokens[p-1].type==TK_POSNUM) ret=ret;
      return ret;
    }
    else if (tokens[p].type==TK_GPR){
      return isa_reg_str2val(tokens[p].str,state);
    }
    else if (tokens[p].type==TK_NUMHEX){
      ret = strtol(tokens[p].str, NULL, 16); //将字符串内的数字转为16进制的数据
      if (tokens[p-1].type==TK_NEGNUM) ret=-ret;
      else if (tokens[p-1].type==TK_POSNUM) ret=ret;
      else if (tokens[p-1].type==TK_POINT){
        return paddr_read(ret,4);
      }
      return ret;
    }
    else {
      state=false;
      return 0;
    }
    return 0;
  } 
  else if (check_parentheses(p, q)) {
    return eval(p+1, q-1, state); //如果有括号，则将p+1及q-1，可以将括号去掉计算
  } 
  else {    //此时的情况就是找到主运算符，然后将各个子式算在一起
    if ((tokens[p].type==TK_NEGNUM)||(tokens[p].type==TK_POSNUM)||(tokens[p].type==TK_POINT)){ //如果這個式子前面的是TK_NEGNUM，則要讓他進行新的計算
      return eval(p+1,q,state);
    }
    int major = find_major(p, q,state); //找到主运算符
    if (*state==false){
      // panic("expr_test error2.2.3!");
      return 0;
    } 

    int val1 = eval(p, major-1, state);
    if (*state==false){
      // panic("expr_test error2.2.4!");
      return 0;
    } 
    int val2 = eval(major+1, q, state);
    if (*state==false){
      // panic("expr_test error2.2.5!");
      return 0;
    } 
    
    switch(tokens[major].type) {
      case '+': 
        return val1 + val2;
      case '-': 
        return val1 - val2;
      case '*': 
        return val1 * val2;
      case '/': 
        if (val2 == 0) {
          *state = false;
          // panic("expr_test error2.2.6!");
          return 0;
        } 
        return (sword_t)val1 / (sword_t)val2;
      case TK_GT:
        if (val1>val2) return 1;
        else return 0;
      case TK_LT:
        if (val1<val2) return 1;
        else return 0;
      case TK_EQ:
        if (val1==val2) return 1;
        else return 0;
      case TK_NEQ:
        if (val1!=val2) return 1;
        else return 0;
      case TK_GE:
        if (val1>=val2) return 1;
        else return 0;
      case TK_LE:
        if (val1<=val2) return 1;
        else return 0;
      case TK_AND:
        if (val1&&val2==1) return 1;
        else return 0;
      case TK_OR:
        if (val1||val2==1) return 1;
        else return 0;
      default: assert(0);
    }
  }
}

int expr(char *e, bool *success) {
  int value;
  if (make_token(e)==false) {
    *success=false;
    return 0;
  }

  for (int i=0;i<nr_token;i++){ //判断这个这个-是负号还是减号
    if ((tokens[i].type=='-')&&(i==0||tokens[i-1].type=='(')) tokens[i].type=TK_NEGNUM;
    else if ((tokens[i].type=='+')&&(i==0||tokens[i-1].type=='(')) tokens[i].type=TK_POSNUM;
    else if ((tokens[i].type=='*')&&(i==0||tokens[i-1].type=='(')) tokens[i].type=TK_POINT;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();
  value=eval(0,nr_token-1,success);
  if (*success==false)
  {
    panic("eval error!");
    return false;
  }
  return value;
}
