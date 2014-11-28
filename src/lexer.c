/*本文件实现词法分析器的功能	时间: 2014.11.25 作者: 唐显斌
 */

#include "../include/lexer.h"


char get_char(void);
void add_char_token_string(char ch);
token get_tocken(void);
char get_char(void);
void back_char(char ch);
void add_char_token_string(char ch);


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
			case '{' : tk.type = L_BRACKET;	return tk;
			case '}' : tk.type = R_BRACKET; return tk;
			case ',' : tk.type = COMMA;		return tk;
			case '+' : tk.type = PLUS;		return tk;
			case '-' : tk.type = MINUS;		return tk;
			case '/' : tk.type = DIV;		return tk;
			/*如果是*的话有两种情况，要分别处理*/
			case '*' :  {
				if ((ch = get_char()) != '*') {
					tk.type = MUL;		
					back_char(ch);
					return tk; 
				} else if (ch == '*') {
					tk.type = POWER;
					return tk;
				}
			}
			default : tk.type = ERRTOKEN;
					  break;
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

