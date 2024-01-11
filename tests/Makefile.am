check_PROGRAMS = criterion

criterion_SOURCES = $(top_builddir)/src/lexer/tests/lexer_tests.c \
					$(top_builddir)/src/io_backend/tests/io_backend_tests.c \
					$(top_builddir)/src/parser/tests/parser_tests.c \
					$(top_builddir)/src/execute/tests/exec_tests.c \
					$(top_builddir)/src/options/tests/options_tests.c

criterion_CPPFLAGS = -I$(top_srcdir)/src

criterion_LDADD = -lcriterion \
				  $(top_builddir)/src/lexer/liblexer.a \
				  $(top_builddir)/src/io_backend/libio.a \
				  $(top_builddir)/src/ast/libast.a \
				  $(top_builddir)/src/parser/libparser.a \
				  $(top_builddir)/src/execute/libexecute.a \
				  $(top_builddir)/src/options/liboptions.a
				  
all: check-local

check-local: criterion
		./criterion
		./testsuite.sh
		
