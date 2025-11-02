#ifndef ENV_H
#define ENV_H

typedef struct EnvVar {
    char* name;
    double value;
    struct EnvVar* next;
} EnvVar;

typedef struct {
    EnvVar* vars;
} Env;

Env* env_new(void);
void env_set(Env* env, const char* name, double value);
double env_get(Env* env, const char* name);
void env_free(Env* env);

#endif

