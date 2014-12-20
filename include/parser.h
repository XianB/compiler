#ifndef PARSER_H_
#define PARSER_H_

#include "../include/lexer.h"
/*函数指针,指向参数为double,
  返回值为double类型的函数
  在本程序中即为sin, cos, tan等函数,在调用这些函数时要用到
 */
typedef double(*func_ptr)(double);

/*
	语法树的节点类型
 */
struct expr_node{
	/*表明该节点的记号类型,参见上机手册的图8.4*/
	enum token_type_t opcode;		/*操作类型, 比如PLUS, MINUS, MUL, DIV, POWER, FUNC, CONST_ID等操作*/

	union {
		/*如果节点类型是操作符的话,就要有左右操作数*/
		struct {
			struct expr_node * left, * right;
		} case_operator;
		/*如果节点类型是函数的话就要有一个孩子作为函数参数,并且要指向要调用的函数地址的指针*/
		struct {
			struct expr_node * child;
			func_ptr math_func_ptr;
		} case_func;
		/*如果节点只是常量*/
		double case_const;
		/*如果节点是参数T的话,T是变量,所以保存的是地址*/
		double * case_parm_ptr;
	}content;
};

void parser(char * src_file);

#endif
