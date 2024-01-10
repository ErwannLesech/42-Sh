CC ?= gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -fsanitize=address -g
CPPFLAGS = -Isrc -lcriterion

OBJ = lexer.o tests/lexer_tests.o 

all: check

check: $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJ) -o check

clean:
	rm -f $(OBJ) check