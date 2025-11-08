#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/parser.h"

static Token current_token;

static Node* parse_expr(void);
static Node* parse_term(void);
static Node* parse_factor(void);

extern Token next_token();
extern void free_token(Token t);

Node* make_node(NodeType type, Node* left, Node* right, double value) {
    Node* n = malloc(sizeof(Node));
    n->type = type;
    n->left = left;
    n->right = right;
    n->value = value;
    n->name = NULL;
    n->op = 0;
    return n;
}

Node* parse_factor() {
    if (current_token.type == TOKEN_NUMBER) {
        Node* node = make_node(NODE_NUMBER, NULL, NULL, current_token.value);
        free_token(current_token);
        current_token = next_token();
        return node;
    }
    else if (current_token.type == TOKEN_STRING) {
        Node* node = make_node(NODE_STRING, NULL, NULL, 0);
        node->strval = strdup(current_token.text);
        free_token(current_token);
        current_token = next_token();
        return node;
    }
    else if (current_token.type == TOKEN_CHAR) {
        Node* node = make_node(NODE_CHAR, NULL, NULL, 0);
        node->strval = strdup(current_token.text);
        free_token(current_token);
        current_token = next_token();
        return node;
    }
    else if (current_token.type == TOKEN_IDENTIFIER) {
        Node* node = make_node(NODE_VAR, NULL, NULL, 0);
        node->name = strdup(current_token.text);
        free_token(current_token);
        current_token = next_token();
        return node;
    }
    else if (current_token.type == TOKEN_LPAREN) {
        free_token(current_token);
        current_token = next_token();
        Node* node = parse_expr();
        if (current_token.type == TOKEN_RPAREN) {
            free_token(current_token);
            current_token = next_token();
        }
        return node;
    }
    return NULL;
}

Node* parse_term() {
    Node* node = parse_factor();
    while (current_token.type == TOKEN_STAR || current_token.type == TOKEN_SLASH) {
        char op = current_token.text[0];
        free_token(current_token);
        current_token = next_token();
        node = make_node(NODE_BINOP, node, parse_factor(), 0);
        node->op = op;
    }
    return node;
}

Node* parse_expr() {
    Node* node = parse_term();
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        char op = current_token.text[0];
        free_token(current_token);
        current_token = next_token();
        node = make_node(NODE_BINOP, node, parse_term(), 0);
        node->op = op;
    }
    return node;
}

Node* parse_statement() {
    if (current_token.type == TOKEN_ASSIGN_KW) {
        free_token(current_token);
        current_token = next_token();
        if (current_token.type != TOKEN_IDENTIFIER) return NULL;

        char* name = strdup(current_token.text);
        free_token(current_token);
        current_token = next_token();

        if (current_token.type != TOKEN_ASSIGN) return NULL;
        free_token(current_token);
        current_token = next_token();

        Node* expr = parse_expr();
        Node* assign = make_node(NODE_ASSIGN, NULL, expr, 0);
        assign->name = name;
        return assign;
    }

    // print <expr>
    if (current_token.type == TOKEN_PRINT) {
        free_token(current_token);
        current_token = next_token();

        NodeList* head = NULL;
        NodeList* tail = NULL;

        // OPCIONAL: permitir print(...)
        bool paren = false;
        if (current_token.type == TOKEN_LPAREN) {
            paren = true;
            free_token(current_token);
            current_token = next_token();
        }

        // Manejar print vacío
        if (paren && current_token.type == TOKEN_RPAREN) {
            free_token(current_token);
            current_token = next_token();

            Node* p = make_node(NODE_PRINT, NULL, NULL, 0);
            p->args = NULL;
            return p;
        }

        // Leer argumentos infinitos
        while (1) {
            Node* expr = parse_expr();
            if (!expr) return NULL;

            NodeList* node = malloc(sizeof(NodeList));
            node->expr = expr;
            node->next = NULL;

            if (!head) head = tail = node;
            else {
                tail->next = node;
                tail = node;
            }

            if (current_token.type == TOKEN_COMMA) {
                free_token(current_token);
                current_token = next_token();
                continue;
            }

            break;
        }

        // Cerrar paréntesis si había
        if (paren) {
            if (current_token.type != TOKEN_RPAREN) return NULL;
            free_token(current_token);
            current_token = next_token();
        }

        Node* print_node = make_node(NODE_PRINT, NULL, NULL, 0);
        print_node->args = head;
        return print_node;
    }


    return parse_expr();
}

Node* parse() {
    current_token = next_token();
    return parse_statement();
}

void free_node(Node* node) {
    if (!node) return;
    free_node(node->left);
    free_node(node->right);
    if (node->name) free(node->name);
    free(node);
}
    
