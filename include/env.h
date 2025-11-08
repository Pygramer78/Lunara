#ifndef ENV_H
#define ENV_H

typedef enum { VAR_NUMBER, VAR_STRING } VarType;

typedef struct EnvVar {
    char* name;
    VarType type;
    double num_value;
    char* str_value;
    struct EnvVar* next;
} EnvVar;

typedef struct {
    EnvVar* vars;
} Env;

Env* env_new(void);
void env_set_num(Env* env, const char* name, double value);
void env_set_str(Env* env, const char* name, const char* value);
EnvVar* env_get_var(Env* env, const char* name);
void env_free(Env* env);

#endif


