#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/eval.h"
#include "../include/env.h"

double eval(Node* node, Env* env) {
    if (!node) return 0.0;

    switch (node->type) {
        case NODE_NUMBER:
            return node->number_value;

        case NODE_STRING:
            /* printing a bare string: print to stdout and return 0 */
            printf("%s\n", node->string_value ? node->string_value : "");
            return 0.0;

        case NODE_CHAR:
            if (node->string_value && node->string_value[0] != '\0')
                printf("%c\n", node->string_value[0]);
            else
                printf("\n");
            return 0.0;

        case NODE_VAR: {
            EnvVar* v = env_get_var(env, node->name);
            if (!v) {
                fprintf(stderr, "Error: variable no definida '%s'\n", node->name);
                exit(1);
            }
            if (v->type == VAR_NUMBER) return v->num_value;
            /* if string, print it and return 0 (or you could error) */
            printf("%s\n", v->str_value ? v->str_value : "");
            return 0.0;
        }

        case NODE_ASSIGN: {
            /* right holds the expression; it can be string or number */
            Node* r = node->right;
            if (!r) return 0.0;
            if (r->type == NODE_STRING || r->type == NODE_CHAR) {
                env_set_str(env, node->name, r->string_value ? r->string_value : "");
            } else {
                double v = eval(r, env);
                env_set_num(env, node->name, v);
                return v;
            }
            return 0.0;
        }

        case NODE_BINOP: {
            double L = eval(node->left, env);
            double R = eval(node->right, env);
            switch (node->op) {
                case '+': return L + R;
                case '-': return L - R;
                case '*': return L * R;
                case '/': return (R != 0.0) ? (L / R) : 0.0;
            }
            return 0.0;
        }

        case NODE_PRINT: {
            NodeList* it = node->args;
            int first = 1;
            if (!it) { /* print() no args -> blank line */ printf("\n"); return 0.0; }
            for (; it; it = it->next) {
                Node* e = it->expr;
                if (!first) printf(" ");
                first = 0;

                if (e->type == NODE_STRING) {
                    printf("%s", e->string_value ? e->string_value : "");
                } else if (e->type == NODE_CHAR) {
                    if (e->string_value && e->string_value[0] != '\0')
                        printf("%c", e->string_value[0]);
                } else if (e->type == NODE_NUMBER) {
                    printf("%g", e->number_value);
                } else if (e->type == NODE_VAR) {
                    EnvVar* v = env_get_var(env, e->name);
                    if (!v) { printf("<undef %s>", e->name); }
                    else {
                        if (v->type == VAR_NUMBER) printf("%g", v->num_value);
                        else printf("%s", v->str_value ? v->str_value : "");
                    }
                } else {
                    double v = eval(e, env);
                    printf("%g", v);
                }
            }
            printf("\n");
            return 0.0;
        }

        default:
            fprintf(stderr, "Error: tipo de nodo desconocido\n");
            exit(1);
    }
}
