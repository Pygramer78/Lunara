#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lunara.h"

int main(int argc, char argv[]) {
    Env* env = env_new();
    char line[256];

    printf("Lunara Interpreter v0.1\n");
    printf("Escribe 'exit' para salir.\n");

    if (argc == 1) {
        FILE lunaFile;
        
    }

    while (1) {
        printf(">>> ");
        if (!fgets(line, sizeof(line), stdin))
            break;

        if (strncmp(line, "exit", 4) == 0)
            break;

        lexer_init(line);
        Node* tree = parse();
        if (!tree) {
            printf("Error de sintaxis.\n");
            continue;
        }

        double result = eval(tree, env);
        printf("= %g\n", result);
        free_node(tree);
    }

    env_free(env);
    return 0;
}

