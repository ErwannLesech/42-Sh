#ifndef BUILTIN_H
#define BUILTIN_H

#include "ast_eval.h"

int echo_fun(struct ast_node *node);

int true_fun(struct ast_node *node);

int false_fun(struct ast_node *node);


#endif /* BUILTIN_H */