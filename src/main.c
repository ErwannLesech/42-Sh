#include "io_backend/io_backend.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "lexer/lexer.h"
#include "execute/ast_eval.h"
#include "options/options.h"

bool logger_enabled = false;

#define LOGGER(...) \
	do { \
		if (logger_enabled) \
			fprintf(stderr, __VA_ARGS__); \
	} while (0)

int main(int argc, char **argv)
{
	// Options handling
	logger_enabled = check_logger(&argc, argv);
	bool pretty_print_enabled = check_pretty_print(&argc, argv);

	char *input = io_backend(argc, argv);

	LOGGER("input: %s\n", input);
	
	struct lexer *lexer = lexer_new(input);

	struct ast_node *ast = parse(lexer);

	if (pretty_print_enabled)
		print_ast(ast, 0);

	match_ast(ast);

	ast_free(ast);
	lexer_free(lexer);
	free(input);
	return 0;
}
