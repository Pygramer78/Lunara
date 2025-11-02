#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    NODE_NUMBER,
    NODE_VAR,
    NODE_BINOP,
    NODE_ASSIGN
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node* left;
    struct Node* right;
    char* name;
    double value;
    char op;
} Node;

Node* parse(void);
void free_node(Node* node);

#endif

