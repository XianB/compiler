/*
	词法分析器头文件
	时间：2014.11.25
	作者：唐显斌
 */

#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#define LEN 100

typedef struct token_t token;
typedef enum token_type_t token_type;
/*记号数据结构中的类别属性的定义，是枚举类型, 每一个记号都有一个对应的类别*/
enum token_type_t {
	/*保留字*/
	ORIGIN, SCALE, ROT, IS,
	TO, STEP, DRAW, FOR, FROM,
	/*参数*/
	T,
	/*分隔符*/
	SEMICO, L_BRACKET, R_BRACKET, COMMA,
	/*运算符*/
	PLUS, MINUS, MUL, DIV, POWER,
	/*函数*/
	FUNC,
	/*常数*/
	CONST_ID,
	/*空记号*/
	NONTOKEN,
	/*错误的记号*/
	ERRTOKEN
};

struct token_t {	/*记号的数据结构*/
	token_type type;		/*记号的类别*/	
	char * lexeme;			/*记号的属性*/
	double value;			/*如果记号是数值则有该值*/
	double (* func_ptr)(double); /*若记号是函数，则是函数的指针*/
};


/*用来区分记号的符号表， 当识别出记号是ID时，用这个表加以区分之,
  因为正则表达式中的各种ID的识别途径是一样的，
  所以需要在识别出记号以后用这个进行进一步的区分
 
 */

/*类型是token类型的*/
#if 0
token token_table[] = {
	{CONST_ID,	"PI",		3.1415926,	NULL},
	{CONST_ID,	"E",		2.718282,	NULL},
	{T,			"T",		0.0,		NULL},
	{FUNC,		"SIN",		0.0,		sin},
	{FUNC,		"COS",		0.0,		cos},
	{FUNC,		"TAN",		0.0,		tan},
	{FUNC,		"LN",		0.0,		log},
	{FUNC,		"exp",		0.0,		exp},
	{FUNC,		"SQRT",		0.0,		sqrt},
	{ORIGIN,	"ORIGIN",	0.0,		NULL},
	{ROT,		"ROT",		0.0,		NULL},
	{IS,		"IS",		0.0,		NULL},
	{FOR,		"FOR",		0.0,		NULL},
	{FROM,		"FROM",		0.0,		NULL},
	{TO,		"TO",		0.0,		NULL},
	{STEP,		"STEP",		0.0,		NULL},
	{DRAW,		"DRAW",		0.0,		NULL},
};
#endif

char get_char(void);
void add_char_token_string(char ch);
extern bool init_scanner(const char * filename);
extern void close_scanner(void);
extern token get_tocken(void);
extern char get_char(void);
extern void back_char(char ch);
extern void add_char_token_string(char ch);
token get_token(void);

#endif











