/*本文件实现词法分析器的功能	时间: 2014.11.25 作者: 唐显斌
 */

#include "../include/lexer.h"
static token token_tab[] = {
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


char get_char(void);
void add_char_token_string(char ch);
token get_tocken(void);
char get_char(void);
void back_char(char ch);
void add_char_token_string(char ch);

token judge_key_token(const char * str);

char tokenbuf[LEN];		/*记号流字符的缓冲区*/
FILE * fp = NULL;
char * filename;
int ps = 0;
int lnum;

token get_token(void)
{

	memset(tokenbuf, 0, LEN);
	ps = 0;
	token tk = {ERRTOKEN, "", 0.0, NULL};	/*当前处理的记号*/
	tk.lexeme = tokenbuf;
	int ch = get_char();			/*get_char函数从输入中获得一个字符*/

	/*过滤掉空白, 如果是换行符则lnum+1 若ch == EOF则退出*/
	while (isspace(ch)) {
		ch = get_char();

		if ( ch == '\n')
			lnum++;
	}

	/*过滤完空格后退出
	 如果是EOF则没找到记号*/
	if ( ch == EOF) {
		tk.type = NONTOKEN;
		return tk;
	}

	/*如果不是EOF则继续*/

	/*add_char_token_string将字符加入到tokenbuffer中*/
	add_char_token_string(ch);	

	/*如果是字符的话就是ID, 使用识别ID的DFA*/
	if (isalpha(ch)) {
		while (isalnum((ch = get_char())))
			add_char_token_string(ch);
		back_char(ch);
		tk = judge_key_token(tokenbuf);
		tk.lexeme = tokenbuf;
		return tk;
	}

	/*如果是数字的话就是数字字面值，使用识别数字的DFA*/
	else if (isdigit(ch)) {
		while (isdigit((ch = get_char()))) {
			add_char_token_string(ch);
		}

		if (ch == '.') {
			add_char_token_string(ch);
			while (isdigit((ch = get_char()))) {
				add_char_token_string(ch);
			}
		}

		back_char(ch);
		tk.type = CONST_ID;
		tk.value = atof(tokenbuf);
		return tk;
	}

	/*否则就是特殊的符号，如分隔符等*/
	else {
		switch (ch) {
			case ';' : tk.type = SEMICO;	return tk;
			case '(' : tk.type = L_BRACKET;	return tk;
			case ')' : tk.type = R_BRACKET; return tk;
			case ',' : tk.type = COMMA;		return tk;
			case '+' : tk.type = PLUS;		return tk;
			/*如果是*的话有两种情况，要分别处理*/
			case '-' :  {
				ch = get_char();
				/*--也是注释符号*/
				if (ch == '-') {
					while (ch != '\n' && ch != EOF) {
						ch = get_char();
						back_char(ch);
						/*本次没有获到token,返回下一个token才行*/
						return get_token();

					}
				} else {
					back_char(ch);
					tk.type = MINUS;
					return tk;;
				}
			}
			case '/': 
					   ch = get_char();
					   if (ch == '/') {
							while (ch != '\n' && ch != EOF) {
								ch = get_char();
								back_char(ch);
								return get_token();
							}
					   } else {
							back_char(ch);
							tk.type = DIV;
							return tk;;
					   }

			case '*':
					   ch = get_char();
					   if (ch == '*') {
							tk.type = POWER;
							return tk;
					   } else {
							back_char(ch);
							tk.type = MUL;
							return tk;
					   }

			default : tk.type = ERRTOKEN;
					  return tk;
		}
	}
	return tk;
}

bool init_scanner(const char * filename)
{
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error while openning file %s\n", filename);
		return false;
	}
	else
		return true;
}

void close_scanner(void)
{
	if (fclose(fp)) {
		fprintf(stderr, "Error while closing file %s\n", filename);
	}
}

/*EOF的判断不在这里进行*/
char get_char(void)
{
	int ch = getc(fp);
	return toupper(ch);
}


void add_char_token_string(char ch)
{
	tokenbuf[ps] = ch;
	tokenbuf[++ps] = 0;
}


void back_char(char ch)
{
	if (ch != EOF)
		ungetc(ch, fp);
}

token judge_key_token(const char * str)
{
	int i;
	for (i = 0; i < sizeof(token_tab) / sizeof(token_tab[0]); i++) {
		if (strcmp(token_tab[i].lexeme, str) == 0)
			return token_tab[i];
	}

	token errtk;
	memset(&errtk, 0, sizeof(token));
	errtk.type = ERRTOKEN;
	return errtk;
}
