#include <stdlib.h>
#include <string.h>
#include "../include/env.h"

Env* env_new(void) {
    Env* e = malloc(sizeof(Env));
    e->vars = NULL;
    return e;
}

void env_set(Env* env, const char* name, double value) {
    EnvVar* var = env->vars;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            var->value = value;
            return;
        }
        var = var->next;
    }

    EnvVar* new_var = malloc(sizeof(EnvVar));
    new_var->name = strdup(name);
    new_var->value = value;
    new_var->next = env->vars;
    env->vars = new_var;
}

double env_get(Env* env, const char* name) {
    EnvVar* var = env->vars;
    while (var) {
        if (strcmp(var->name, name) == 0)
            return var->value;
        var = var->next;
    }
    return 0.0;
}

void env_free(Env* env) {
    EnvVar* var = env->vars;
    while (var) {
        EnvVar* next = var->next;
        free(var->name);
        free(var);
        var = next;
    }
    free(env);
}

