#include <stdio.h>
#include <ctype.h>

int main(int argc, char ** argv) {

	char ch;
	char * buf = argv[1];
	int i = 0;

	if (isdigit(buf[i])) {
		while (isdigit((buf[i++])))
			;
		i--;
//		printf("\n\n %c \n\n", buf[i]);

		if (buf[i] == '.') {
				while (isdigit(buf[i++]))
					;
		} else if (buf[i] != 0) {
			printf("no\n");
			return 0;
		}
		printf("yes\n");
		return 0;
	}

	printf("no\n");
	return 0;
}
