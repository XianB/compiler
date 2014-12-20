INCLUDES=-I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib

sbin_PROGRAMS = lexer parser

lexer_SOURCES = lexer_driver.c $(top_srcdir)/src/lexer.c
lexer_LDADD = -lm

parser_SOURCES = parser_driver.c $(top_srcdir)/src/parser.c $(top_srcdir)/src/lexer.c
parser_LDADD = -lm
