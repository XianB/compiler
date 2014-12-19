#include "../include/parser.h"

static token tk;
double parameter = 0;

/*一些辅助的宏函数*/
#define enter(x) printf("enter in "); printf(x); printf("\n");
#define back(x) printf("exit from "); printf(x); printf("\n");
#define call_match(x) printf("match_token "); printf(x); printf("\n");
#define tree_trace(x) print_syntax_tree(x, 1);		/*打印出表达式的语法树*/


/*辅助函数的声明*/
static void fetch_token(void);		/*从词法分析器中获得一个记号*/
static void match_token(enum token_type atk); /*匹配一个记号*/
static void syntax_error(int case_of);			/*报告语法错误*/
static err_msg(unsigned line_no, char * descrip, char * string);
static void print_syntax_tree(struct expr_node * root, int indent);

/*非终结符的递归子程序声明*/
static void program(void);
static void statement(void);
static void origin_statement(void);
static void rot_statement(void);
static void scale_statement(void);
static void for_statement(void);
static struct expr_node* expression(void);
static struct expr_node* term(void);
static struct expr_node* factor(void);
static struct expr_node * component(void);
static struct expr_node * atom(void);

/*外部接口与语法树构造函数声明*/
extern void parser(char * src_filename);
static struct expr_node *make_expr_node(enum token_type opcode, ...);

/*通过词法分析器接口get_token函数获取一个记号*/
static void fetch_token(void);
{
	tk = get_token();
	if (token.type == ERRTOKEN)
		syntax_error(1);
}

/*匹配记号,如果不匹配则为语法错误,如果匹配则继续获取下一个记号*/
static void match_token(enum token_type the_token)
{
	if (tk.type != the_token)
		syntax_error(2);
	fetch_token();
}

/*语法错误情况的处理*/
static void syntax_error(int case_of)
{
	switch(case_of) {
		case 1: 
				err_msg(line_no, "错误的记号", tk.lexeme);
				break;
		case 2:
				err_msg(line_no, "不是预期的记号", tk.lexeme);
				break;

	}
}


/*打印错误信息*/
void err_msg(unsigned line_no, char * descrip, char * string)
{
	printf("line no %5d: %s %s!\n", line_no, descrip, string);
	close_scanner();
	exit(1);
}

/*先序遍历并打印表达式的语法树*/
void print_syntax_tree(struct expr_node * root, int indent)
{
	int temp;

	for (temp = 1; temp <= indent; temp++) {
		printf("\t");	/*缩进*/
		switch(root->opcode) {
			case PLUS:
				printf("%s\n", "+");
				break;
			case MINUS:
				printf("%s\n", "-");
				break;
			case MUL:
				printf("%s\n", "*");
				break;
			case DIV:
				printf("%s\n", "/");
				break;
			case POWER:
				printf("%s\n", "**");
				break;
			case FUNC:
				printf("%x\n", root->content.case_func.match_func_ptr);
				break;
			case CONST_ID:
				printf("%f\n", root->content.case_const);
				break;
			case T:
				printf("%s\n", "T");
				break;
			default:
				printf("ERROR TREE NODE!\n");
				exit(0);
		}
	}

	/*叶子节点了,直接结束,不再递归分析子树了~因为没有子树了*/
	if (root->opcode == CONST_ID || root->opcode == T)
		return;
	/*非叶子节点:函数*/
	if (root->opcode == FUNC)
		print_syntax_tree(root->content.case_func.child, indent + 1);
	/*非叶子节点:操作符*/
	else {
		print_syntax_tree(root->content.case_operator.left, indent + 1);
		print_syntax_tree(root->content.case_operator.right, indent + 1);
	}
}

void praser(char * src_filename)
{
	enter("parser");
	if (!init_scanner(src_filename)) {
		printf("open source file:%s error!\n", src_filename);
		return ;
	}
	fetch_token();
	program();		/*开始递归下降分析*/
	close_scanner();
	back("parser");
	return;
}


/*program 的递归子程序*/
static void program(void)
{
	enter("program");
	/*program的文法是
		program --> {statement SEMICO}
		参照手册P75
	 */
	while (tk.type != NONTOKEN){
		/*开始语句分析*/
		statement();
		/*每个语句以分号结束*/
		matck_token(SEMICO);
	}
	back("program");
}

/*statement 的递归子程序*/
static void statement(void)
{
	/*statement的文法
	  statement --> origint_statement | scale_statement | rot_statement | for_statement
	 */
	enter("statement");

	switch(tk.type) {
		case ORIGIN:
			origin_statement();
			break;
		case SCALE:
			scale_statement();
			break;
		case ROT:
			rot_statement();
			break;
		case FOR:
			for_statement();
			break;
		default:
			/*如果开始的记号不是语句的话就是语法错误了~*/
			syntax_error(2);
	}
	back("statement");
}

/*origin_statement的递归子程序*/
static void origin_statement(void)
{
	/*origin设置初始化位置,所以需要两个表达式,一个计算出x,一个计算出y*/
	/*origin_statement的文法是
	 origin_statement -- > ORIGIN IS L_BRACKET expression COMMA expression R_BRACKET
	 */
	struct expr_node * tmp;

	enter("origin_statement");
	match_token(ORIGIN);
	match_token(IS);
	match_token(L_BRACKET);
	tmp = expression();		/*在expression里会调用tree_trace来打印表达式树的,注意这里是语法分析,所以不需要计算出结果来*/
	match_token(COMMA);
	tmp = expression();
	match_token(R_BRACKET);
	back("origin_statement");
}


/*scale_statement的递归子程序*/
static void scale_statement(void)
{
	/*scale_statement设置比例因子,可以同时设置x和y的比例变换因子*/

	/*scale_statement 的文法为
		scale_statement --> SCALE IS L_BRACKET expression COMMA expression R_BRACKET
	 */
	struct expr_node * tmp;
	enter("scale_statement");

	match_token(SCALE);
	match_token(IS);
	match_token(L_BRACKET);
	tmp = expression();
	match_token(COMMA);
	tmp = expression();

	match_token(R_BRACKET);
	back("scale_statement");
}


/*rot_statement的递归子程序*/
static void rot_statement(void)
{
	/*rot_statement实现图像的旋转*/
	/*rot_statement 的文法是
		rot_statement --> ROT IS expression
	 */
	struct expr_node * tmp;

	enter("rot_statement");
	match_token(ROT);
	match_token(IS);
	tmp = expression();
	back("rot_statement");
}

/*for_statement的递归子程序*/
static void for_statement(void)
{
	/*for_statement 实现for循环语句,一定有T参数*/
	/*
	   for_statement --> FOR T FROM expression TO expression STEP expression DRAW L_BRACKET expression COMMA expression R_BRACKET
	 
	 */

	struct expr_node * star_ptr, *end_prt, * step_prt, *x_ptr, *y_ptr;

	enter("for_statement");
	match_token(FOR);
	call_match("FOR");
	match_token(T);
	call_match("T");
	match_token(FROM);
	call_match("FROM");
	start_ptr = expression();
	match_token(TO);
	call_match("TO");
	end_ptr = expression();
	match_token(STEP);
	call_match("STEP");
	step_ptr = expression();
	match_token(DRAW);
	call_match("DRAW");
	match_token(L_BRACKET);
	call_match("(");
	x_ptr = expression();
	match_token(COMMA);
	call_match(",");
	y_ptr = expression();
	match_token(R_BRACKET);
	call_match(")");
	back("for_statement");
}


/*******************至此语句分析各个函数完成*********************************************************************/





/****************************************************************************************************************/
/*expression 的递归子程序*/
static struct expr_node* expression(void)
{
	/*expression 构造表达式树,并且打印表达式树*/

	/*
	   expression --> term {(PLUS|MINUS) term}
	 
	   引入term的目的是提供优先级以及结合性的区分
	   term含义为非终结符
	   atom为原子表达式
	   见P72
	 */
	/*左右子树节点的指针*/
	struct expr_node * left, * right;
	/*当前记号*/
	token_type tk_tmp;

	enter("expression");
	/*分析左操作数得其语法树*/
	left = term();
	while(tk.type == PLUS || tk.type == MINUS) {
		tk_tmp = tk.type;
		match_token(tk_tmp);
		/*分析右操作数得到其语法树*/
		right = term();
		/*构造运算的语法树,结果为左子树*/
		left = make_expr_node(tk_tmp, left, right);

	}

	tree_trace(left);		/*打印表达式的语法树*/
	back("expression");
	return left;		/*为什么返回左子树?*/
}

/*term 的递归子程序*/
static struct expr_node * term(void)
{
	/*term -- >factor{(MUL|DIV)factor}*/
	struct expr_node * left, * right;
	token_type tk_tmp;

	left = factor();
	while (tk.type == MUL || tk.type == DIV) {
		tk_tmp = tk.type;
		match_token(tk_tmp);
		right = factor();
		left = make_expr_node(tk_tmp, left, right);
	}
	return left;
}
/*factor 的递归子程序*/
static struct expr_node * factor(void)
{
	/*
	 factor --> PLUS factor | MINUS factor | component
	 */
	struct expr_node * left, right;

	/*匹配一元加运算*/
	if (tk.type == PLUS) {
		match_token(PLUS);
		/*表达式退化为仅有右操作数的表达式*/
		right = factor();
	} else if(tk.type == MINUS) {
		/*匹配一元减运算,表达式转化为二元减运算的表达式,即0-..*/
		match_token(MINUS);
		right = factor();
		left = (struct expr_node *)malloc(sizeof(struct expr_node));
		left->opcode = CONST_ID;
		left->content.case_const = 0.0;
		right = make_expr_node(MINUS, left, right);
		
	} else right = component();			/*匹配非终结符component*/

	return right;
}


/*component 的递归子程序*/
static struct expr_node* component(void)
{
	/*component --> atom[POWER component]*/
	struct expr_node * left, * right;

	left = atom();
	if (tk.type == POWER) {
		match_token(POWER);
		/*递归调用component以实现POWER的右结合,这里的表现方法既是,先把component计算完整的才行,所以要递归调用
		 如2**3**4
		 则是先算t = 3**4
		 再算2**t
		 */
		right = component();
		left = make_expr_node(POWER, left, right);
	}
	return left;
}


/*atom 的递归子程序*/
static struct expr_node * atom(void)
{
	/*原子表达式,为原子表达式建立表达式树*/
	/*
	   atom --> CONST_ID | T | FUNC L_BRACKET expression R_BRACKET | L_BRACKET expression R_BRACKET
	 */
	struct token t = tk;
	struct expr_node * address, * tmp;

	switch(tk.type) {
		case CONST_ID:
			match_token(CONST_ID);
			address = make_expr_node(CONST_ID, t.value);
			break;
		case T:
			match_token(T);
			address = make_expr_node(T);
			break;
		case FUNC:
			match_token(FUNC);
			match_token(L_BRACKET);
			tmp = expression();
			address = make_expr_node(FUNC, t.func_ptr, tmp);
			match_token(R_BRACKET);
			break;
		case L_BRACKET:
			match_token(L_BRACKET);
			address = expression();
			match_token(R_BRACKET);
			break;
		default:
			syntax_error(2);
	}
	return address;
}


/*生成语法树的一个节点*/
static struct expr_node * make_expr_node(enum token_type opcode, ...)
{
	struct expr_node * expr_ptr = (struct expr_node *)malloc(sizeof(struct expr_node));
	expr_ptr->opcode = opcode;
	va_list arg_ptr;
	va_start(arg_ptr, opcode);
	switch(opcode) {
		case CONST_ID:
			expr_ptr->content.case_const = (double)va_arg(arg_ptr, double);
			break;
		case T:
			expr_ptr->content.case_parm_ptr = &parameter;
			break;
		case FUNC:
			expr_ptr->content.case_func.math_cunt_ptr = (func_ptr)va_arg(arg_ptr, func_ptr);
			expr_ptr->content.case_func.child = (struct expr_node *) va_arg(arg_ptr, struct expr_node*);
			break;
		default:
			expr_ptr->content.case_operator.left = (struct expr_node*) va_arg(arg_ptr, struct expr_node*);
			expr_ptr->content.case_operator.right = (struct expr_node*) va_arg(arg_ptr, struct expr_node*);
			break;
	}
	va_end(arg_ptr);
	return expr_ptr;
}
