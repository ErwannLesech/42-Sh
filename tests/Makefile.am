check_PROGRAMS = criterion

criterion_SOURCES = $(top_builddir)/src/lexer/tests/lexer_tests.c
criterion_CPPFLAGS = -I$(top_srcdir)/src

criterion_LDADD = -lcriterion \
                  $(top_builddir)/src/lexer/liblexer.a

check-local: criterion
		./criterion
		./test.sh $(top_builddir)/42sh
