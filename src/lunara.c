#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lunara.h"

int main(int argc, char** argv) {
    Env* env = env_new();
    char line[1024];

    /* si pasa un archivo, ejecutar su contenido */
    if (argc > 1) {
        FILE* f = fopen(argv[1], "rb");
        if (!f) { perror("fopen"); return 1; }
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        rewind(f);
        char* code = malloc(size + 1);
        fread(code, 1, size, f);
        code[size] = '\0';
        fclose(f);

        lexer_init(code);

        while (1) {
            Node* t = parse();
            if (!t) break;
            if (t->type == NODE_NUMBER && t->number_value == 0 &&
                t->left == NULL && t->right == NULL) {
                // pequeña protección contra parse vacío
                free_node(t);
                break;
            }

            eval(t, env);
            free_node(t);

            /* Stop when EOF token was consumed */
            extern Token current_token; // si no lo tienes, lo exponemos
            if (current_token.type == TOKEN_EOF)
                break;
        }

        free(code);
        env_free(env);
        return 0;
    }

    printf("Lunara REPL v0.1 — write 'exit' to quit\n");

    while (1) {
        printf(">>> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        if (strncmp(line, "exit", 4) == 0) break;
        if (line[0] == '\n' || line[0] == '\0') continue;

        lexer_init(line);
        Node* tree = parse();
        if (!tree) { printf("Syntax error\n"); continue; }

        double r = eval(tree, env);
        /* print numeric result only if expression yields number and not print/assign */
        (void)r;
        free_node(tree);
    }

    env_free(env);
    return 0;
}
