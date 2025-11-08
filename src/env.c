#include <stdlib.h>
#include <string.h>
#include "../include/env.h"

Env* env_new(void) {
    Env* e = malloc(sizeof(Env));
    e->vars = NULL;
    return e;
}

EnvVar* env_get_var(Env* env, const char* name) {
    EnvVar* v = env->vars;
    while (v) {
        if (strcmp(v->name, name) == 0) return v;
        v = v->next;
    }
    return NULL;
}

void env_set_num(Env* env, const char* name, double value) {
    EnvVar* v = env_get_var(env, name);
    if (v) {
        if (v->type == VAR_STRING) {
            free(v->str_value);
            v->str_value = NULL;
        }
        v->type = VAR_NUMBER;
        v->num_value = value;
        return;
    }
    EnvVar* nv = malloc(sizeof(EnvVar));
    nv->name = strdup(name);
    nv->type = VAR_NUMBER;
    nv->num_value = value;
    nv->str_value = NULL;
    nv->next = env->vars;
    env->vars = nv;
}

void env_set_str(Env* env, const char* name, const char* value) {
    EnvVar* v = env_get_var(env, name);
    if (v) {
        if (v->type == VAR_STRING) {
            free(v->str_value);
        } else {
            v->num_value = 0.0;
        }
        v->type = VAR_STRING;
        v->str_value = strdup(value);
        return;
    }
    EnvVar* nv = malloc(sizeof(EnvVar));
    nv->name = strdup(name);
    nv->type = VAR_STRING;
    nv->num_value = 0.0;
    nv->str_value = strdup(value);
    nv->next = env->vars;
    env->vars = nv;
}

void env_free(Env* env) {
    EnvVar* v = env->vars;
    while (v) {
        EnvVar* nxt = v->next;
        free(v->name);
        if (v->type == VAR_STRING && v->str_value) free(v->str_value);
        free(v);
        v = nxt;
    }
    free(env);
}
