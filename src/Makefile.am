SUBDIRS = lexer \
        ast \
        parser \
        io_backend \
        execute \
        options

bin_PROGRAMS = 42sh

42sh_SOURCES = main.c

42sh_CPPFLAGS = -I%D%

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -fsanitize=address -g

42sh_LDADD =  lexer/liblexer.a \
       ast/libast.a \
       parser/libparser.a \
       io_backend/libio.a \
       execute/libexecute.a \
       options/liboptions.a
