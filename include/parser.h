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

#include "ast.h"
#include "lexer.h"
extern Token current_token;
Node* parse(void);
void free_node(Node* node);

#endif

