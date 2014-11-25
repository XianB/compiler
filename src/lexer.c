/*本文件实现词法分析器的功能
	时间: 2014.11.25
	作者: 唐显斌
 */

#include "../include/lexer.h"

token lexer(void)
{

	char tokenbuf[LEN];		/*记号流字符的缓冲区*/
	struct token tk = {"", ERRTOKEN, 0.0, NULL};	/*当前处理的记号*/
	token.lexeme = tokenbuf;
	char ch = get_char();			/*get_char函数从输入中获得一个字符*/

	/*过滤掉空白*/
	while (isspace(ch)) {
		ch = get_char();
	}

	/*add_char_token_string将字符加入到tokenbuffer中*/
	add_char_token_string(ch);	

	/*如果是字符的话就是ID, 使用识别ID的DFA*/
	if (isalpha(ch)) {
	
	
	}

	/*如果是数字的话就是数字字面值，使用识别数字的DFA*/
	else if (isdigit(ch)) {
	
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
		}
	}
}
