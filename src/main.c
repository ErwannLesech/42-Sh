#include "io_backend/io_backend.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "lexer/lexer.h"

int main(int argc, char **argv)
{
	char *input = io_backend(argc, argv);

	printf("%s\n", input);
	
	struct lexer *lexer = lexer_new(input);

	struct ast_node *ast = parse(lexer);

	print_ast(ast, 0);

	ast_free(ast);
	lexer_free(lexer);
	free(input);
	return 0;
}
