lib_LIBRARIES = libexecute.a

libexecute_a_SOURCES = ast_eval.c ast_eval.h \
                    ../parser/parser.h ../ast/ast.h \
                    utils/builtin.c utils/builtin.h \
                    utils/builtin_path.c \
                    utils/pipeline.c utils/loop.c \
                    hash_map/hash_map.c hash_map/hash_map.h \
                    hash_map/hash.c utils/ast_variable.c \
                    utils/environment_variable.c utils/environment_variable.h \
                    utils/redirections.c
                

libexecute_a_CFLAGS = -Wall -Wextra -Werror -Wvla -pedantic -std=c99
libexecute_a_CPPFLAGS =-I$(top_srcdir)/src
