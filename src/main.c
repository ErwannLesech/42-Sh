#include "io_backend/io_backend.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "lexer/lexer.h"
#include "execute/ast_eval.h"
#include "options/options.h"

#include <err.h>

int main(int argc, char **argv)
{
	// Options handling
	bool logger_enabled = check_logger(&argc, argv);
	bool pretty_print_enabled = check_pretty_print(&argc, argv);

	char *input = io_backend(argc, argv);
	if (input == NULL)
	{
		errx(1, "Error while reading input");
	}

	logger(input, LOGGER_INPUT, logger_enabled);
	
	struct lexer *lexer = lexer_new(input);

	int val = parser_loop(lexer, logger_enabled, pretty_print_enabled);

	lexer_free(lexer);
	free(input);
	return val;
}
