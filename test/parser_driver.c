/*
	语法分析器的驱动程序,用来测试parser.c程序的功能
 */

#include <stdio.h>
#include "../include/parser.h"

//void parser(char * file);

int main(int argc, char ** argv)
{
	if (argc < 2) {
		printf("usage: %s test_filename\n", argv[0]);
		return 0;
	}

	parser(argv[1]);
	return 0;
}
