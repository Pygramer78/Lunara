#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER,
    NODE_STRING,
    NODE_CHAR,
    NODE_VAR,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_PRINT
} NodeType;

typedef struct Node Node;

typedef struct NodeList {
    Node* expr;
    struct NodeList* next;
} NodeList;

struct Node {
    NodeType type;
    Node* left;
    Node* right;

    /* valores */
    double number_value;   /* para NODE_NUMBER */
    char* string_value;    /* para NODE_STRING o NODE_CHAR (string of length 1) */

    /* para variables / assign */
    char* name;

    /* para binop */
    char op;

    /* para llamadas tipo print(...) */
    NodeList* args;
};

#endif
