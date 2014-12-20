#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(compiler, 1.0, txianb@gmail.com)
#srcdir用来侦测所指定的源码文件是否存在从而确定目录的有效性,哪个都一样的
AC_CONFIG_SRCDIR([include/parser.h])
#header用于生成config.h文件,以便autoheader命令使用
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(compiler, 1.0)

# Checks for programs.
#用来指定编译器
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

#用来设定configure所要产生的文件,如果是makefile,configure会把他检查保护来的结果带入makefile.in文件产生合适的makefile,使用automake时,还需要其他的参数
#AC_CONFIG_FILES([Makefile])
AC_OUTPUT([
		Makefile
		test/Makefile
		src/Makefile
		include/Makefile
		])
