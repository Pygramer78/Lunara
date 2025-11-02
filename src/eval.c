#include <stdio.h>
#include <stdlib.h>
#include "../include/eval.h"

double eval(Node* node, Env* env) {
    switch (node->type) {
        case NODE_NUMBER:
            return node->value;
        case NODE_VAR:
            return env_get(env, node->name);
        case NODE_ASSIGN: {
            double val = eval(node->right, env);
            env_set(env, node->name, val);
            return val;
        }
        case NODE_BINOP: {
            double left = eval(node->left, env);
            double right = eval(node->right, env);
            switch (node->op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': return right != 0 ? left / right : 0;
            }
        }
        default:
            fprintf(stderr, "Error: tipo de nodo desconocido\n");
            exit(1);
    }
}

