#include <stdio.h>
#include <stdlib.h>
#include "../include/eval.h"

double eval(Node* node, Env* env) {
    switch (node->type) {
        case NODE_NUMBER:
            return node->value;

        case NODE_STRING:
        case NODE_CHAR:
            printf("%s\n", node->strval);
            return 0;

        case NODE_VAR: {
            EnvVar* var = env_get_var(env, node->name);
            if (!var) {
                fprintf(stderr, "Error: variable '%s' no definida\n", node->name);
                exit(1);
            }
            if (var->type == VAR_NUMBER)
                return var->num_value;
            else {
                printf("%s\n", var->str_value);
                return 0;
            }
        }

        case NODE_ASSIGN: {
            if (node->right->type == NODE_STRING || node->right->type == NODE_CHAR)
                env_set_str(env, node->name, node->right->strval);
            else
                env_set_num(env, node->name, eval(node->right, env));
            return 0;
        }

        case NODE_BINOP: {
            double l = eval(node->left, env);
            double r = eval(node->right, env);
            switch (node->op) {
                case '+': return l + r;
                case '-': return l - r;
                case '*': return l * r;
                case '/': return r != 0 ? l / r : 0;
            }
        }

        case NODE_PRINT: {
            NodeList* arg = node->args;

            while (arg) {
                Node* e = arg->expr;
                
                switch (e->type) {

                    case NODE_STRING:
                        printf("%s", e->strval);
                        break;

                    case NODE_CHAR:
                        printf("%c", (char)e->value);
                        break;

                    case NODE_NUMBER:
                        printf("%g", e->value);
                        break;

                    default: {
                        double v = eval(e, env);
                        printf("%g", v);
                    }
                }

                if (arg->next)
                    printf(" ");

                arg = arg->next;
            }

            printf("\n");
            return 0;
        }
        
        default:
            fprintf(stderr, "Error: unknown node type\n");
            exit(1);
    }
}


