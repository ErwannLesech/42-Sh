/**
 * \file parser.h
 * \brief Header for the parser.
 * \authors Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef PARSER_H
#define PARSER_H

#include <string.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"

/**
 * \brief Parse loop the given lexer
 * \param lexer The lexer to parse.
 * \param logger_enabled The option logger.
 * \param pretty_print_enabled The option pretty_print.
 * \return code of execution.
 */
int parser_loop(struct lexer *lexer, bool logger_enabled,
                bool pretty_print_enabled);

/**
 * \brief Parse the given lexer.
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *parse(struct lexer *lexer);

/**
 * \brief shell_command = rule_if ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *shell_command(struct lexer *lexer);

/**
 * \brief rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi';
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *rule_if(struct lexer *lexer);

/**
 * \brief else_clause = 'else' compound_list
    | 'elif' compound_list 'then' compound_list [else_clause] ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *else_clause(struct lexer *lexer);

/**
 * \brief compound_list = {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or } [';']
 * {'\n'} ; \param lexer The lexer to parse. \return A pointer to the AST.
 */
struct ast_node *compound_list(struct lexer *lexer);

/**
 * \brief input = list '\n' | list EOF | '\n' | EOF ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *input(struct lexer *lexer);

/**
 * \brief list = and_or { ';' and_or } [ ';' ] ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *list(struct lexer *lexer);

/**
 * \brief and_or = pipeline ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *and_or(struct lexer *lexer);

/**
 * \brief pipeline = command ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *pipeline(struct lexer *lexer);

/**
 * \brief command = simple_command | shell_command ;
 *  \brief IMPORTANT !!! AST_COMMAND CAN HANDLE REDIRECTION !!
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *command(struct lexer *lexer);

/**
 * \brief simple_command = WORD { element } ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *simple_command(struct lexer *lexer);

/**
 * \brief element = WORD ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *element(struct lexer *lexer);

/**
 * \brief  redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' |
 * '<>' ) WORD ; \param lexer The lexer to parse. \return A pointer to the AST.
 */
struct ast_node *redirection(struct lexer *lexer);

/**
 * \brief  rule_while = 'while' compound_list 'do' compound_list 'done' ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *rule_while(struct lexer *lexer);

/**
 * \brief  rule_until = 'until' compound_list 'do' compound_list 'done' ;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *rule_until(struct lexer *lexer);

/**
 * \brief  rule_for = 'for' WORD [in WORD {',' WORD}] 'do' compound_list 'done'
 * ; \param lexer The lexer to parse. \return A pointer to the AST.
 */
struct ast_node *rule_for(struct lexer *lexer);
/**
 * \brief  redirection;
 * \param lexer The lexer to parse.
 * \return A pointer to the AST.
 */
struct ast_node *prefix(struct lexer *lexer);
/**
 * \brief Return the next token type without consuming it.
 * \param lexer The lexer to parse.
 * \return The next token type.
 */
enum token_type parser_peek(struct lexer *lexer);

/**
 * \brief Return the next token type and consume it.
 * \param lexer The lexer to parse.
 * \return The next token type.
 */
enum token_type parser_pop(struct lexer *lexer);

struct ast_node *parse_variable(struct lexer *lexer);

#endif /* ! PARSER_H */