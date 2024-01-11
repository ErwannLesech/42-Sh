lib_LIBRARIES = libparser.a

libparser_a_SOURCES = parser.c parser.h \
                    parser_condition.c parser_element.c \
                    ../ast/ast.c ../ast/ast.h \
                    ../lexer/lexer.c ../lexer/lexer.h

libparser_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic 
libparser_a_CPPFLAGS =-I$(top_srcdir)
