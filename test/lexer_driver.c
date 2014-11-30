#include "../include/lexer.h"

extern char * filename;
int main(int argc, char **argv)
{
	filename = argv[1];
	init_scanner(filename);

	printf("type		lexeme		value		func\n");
	while (1) {
		token tk = get_token();
		if (tk.type == NONTOKEN)
			break;
		printf("%d		%s		%lf		%x\n", tk.type, tk.lexeme, tk.value, tk.func_ptr);
	}

	return 0;
}
