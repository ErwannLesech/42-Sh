lib_LIBRARIES = libexecute.a

libexecute_a_SOURCES = ast_eval.c ast_eval.h ../parser/parser.h \
                    builtin.c builtin.h \
                    ../ast/ast.h 
                

libexecute_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic 
libexecute_a_CPPFLAGS =-I$(top_srcdir)