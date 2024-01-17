lib_LIBRARIES = libexecute.a

libexecute_a_SOURCES = pipeline.c loop.c ast_eval.c ast_eval.h ../parser/parser.h \
                    builtin.c builtin.h \
                    ../ast/ast.h hash_map/hash_map.c hash_map/hash_map.h hash_map/hash.c ast_variable.c
                

libexecute_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic 
libexecute_a_CPPFLAGS =-I$(top_srcdir)