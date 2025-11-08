#ifndef EVAL_H
#define EVAL_H

#include "ast.h"
#include "env.h"

double eval(Node* node, Env* env);

#endif
