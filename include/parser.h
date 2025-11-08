/*
parser.h: File for lunara's code parsing.
Lunara works by TOKENS and NODES, which is a way
to manage functions. Every keyword has a TOKEN and a NODE,
All tokens belong to the file lexer.h

For more details of all functions go to ../src/parser.c
I have to say that this languaje was created by a Spanish person,
so every comment is in spanish.
*/


#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct NodeList {
    Node* expr;
    struct NodeList* next;
} NodeList;


typedef enum {
    NODE_NUMBER,
    NODE_STRING,
    NODE_CHAR,
    NODE_VAR,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_PRINT
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node* left;
    struct Node* right;
    // Campo de variables
    char* name;
    double value;
    char* strval;  // nuevo campo para strings
    char op;

    // Campo de builtins
    NodeList* args; // <-- De print
} Node;


Node* parse(void);
void free_node(Node* node);

#endif

