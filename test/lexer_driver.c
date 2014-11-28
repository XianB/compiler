#include "../include/lexer.h"

extern char * filename;
int main(int argc, char **argv)
{
	filename = argv[1];
	init_scanner(filename);

	int i = 8;
	printf("type		lexeme		value		func\n");
	while (i-- > 0) {
		token tk = get_token();
	printf("%d		%s		%lf	\n", tk.type, tk.lexeme, tk.value);
	}

	return 0;
}
