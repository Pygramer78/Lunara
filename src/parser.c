#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/lexer.h"
#include <stdio.h>

/* estado interno del parser */
Token current_token;

/* funciones helper (no exportadas) */
static Node* parse_expr(void);
static Node* parse_term(void);
static Node* parse_factor(void);

extern Token next_token();
extern void free_token(Token t);

/* fabrica nodos */
static Node* make_node(NodeType type) {
    Node* n = malloc(sizeof(Node));
    n->type = type;
    n->left = n->right = NULL;
    n->number_value = 0.0;
    n->string_value = NULL;
    n->name = NULL;
    n->op = 0;
    n->args = NULL;
    return n;
}

/* libera NodeList recursivamente */
static void free_nodelist(NodeList* l) {
    while (l) {
        NodeList* next = l->next;
        free_node(l->expr);
        free(l);
        l = next;
    }
}

/* implementación de free_node pública */
void free_node(Node* node) {
    if (!node) return;
    if (node->name) free(node->name);
    if (node->string_value) free(node->string_value);
    free_node(node->left);
    free_node(node->right);
    free_nodelist(node->args);
    free(node);
}

/* grammar:
   statement -> 'assign' IDENT '=' expr
              | 'print' ( expr (',' expr)* | '(' ( expr (',' expr)* )? ')' )
              | expr
   expr -> term (('+'|'-') term)*
   term -> factor (('*'|'/') factor)*
   factor -> NUMBER | STRING | CHAR | IDENT | '(' expr ')'
*/

/* parse_factor */
static Node* parse_factor(void) {
    if (current_token.type == TOKEN_NUMBER) {
        Node* n = make_node(NODE_NUMBER);
        n->number_value = current_token.value;
        free_token(current_token);
        current_token = next_token();
        return n;
    }
    if (current_token.type == TOKEN_STRING) {
        Node* n = make_node(NODE_STRING);
        n->string_value = strdup(current_token.text);
        free_token(current_token);
        current_token = next_token();
        return n;
    }
    if (current_token.type == TOKEN_CHAR) {
        Node* n = make_node(NODE_CHAR);
        n->string_value = strdup(current_token.text); /* length 1 string */
        free_token(current_token);
        current_token = next_token();
        return n;
    }
    if (current_token.type == TOKEN_IDENTIFIER) {
        Node* n = make_node(NODE_VAR);
        n->name = strdup(current_token.text);
        free_token(current_token);
        current_token = next_token();
        return n;
    }
    if (current_token.type == TOKEN_LPAREN) {
        free_token(current_token);
        current_token = next_token();
        Node* n = parse_expr();
        if (current_token.type == TOKEN_RPAREN) {
            free_token(current_token);
            current_token = next_token();
        } else {
            /* missing ')' - tolerate */
        }
        return n;
    }
    return NULL;
}

/* parse_term */
static Node* parse_term(void) {
    Node* node = parse_factor();
    while (current_token.type == TOKEN_STAR || current_token.type == TOKEN_SLASH) {
        char op = current_token.text[0];
        free_token(current_token);
        current_token = next_token();
        Node* right = parse_factor();
        Node* n = make_node(NODE_BINOP);
        n->left = node;
        n->right = right;
        n->op = op;
        node = n;
    }
    return node;
}

/* parse_expr */
static Node* parse_expr(void) {
    Node* node = parse_term();
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        char op = current_token.text[0];
        free_token(current_token);
        current_token = next_token();
        Node* right = parse_term();
        Node* n = make_node(NODE_BINOP);
        n->left = node;
        n->right = right;
        n->op = op;
        node = n;
    }
    return node;
}

/* parse_statement (public parse() wrapper uses this) */
static Node* parse_statement_internal(void) {
    /* print ... */
    if (current_token.type == TOKEN_PRINT) {
        free_token(current_token);
        current_token = next_token();

        int paren = 0;
        if (current_token.type == TOKEN_LPAREN) {
            paren = 1;
            free_token(current_token);
            current_token = next_token();
        }

        NodeList* head = NULL;
        NodeList* tail = NULL;

        /* handle empty print() */
        if (paren && current_token.type == TOKEN_RPAREN) {
            free_token(current_token);
            current_token = next_token();
            Node* pnode = make_node(NODE_PRINT);
            pnode->args = NULL;
            return pnode;
        }

        /* read first/next args */
        while (1) {
            Node* expr = parse_expr();
            if (!expr) break;
            NodeList* it = malloc(sizeof(NodeList));
            it->expr = expr;
            it->next = NULL;
            if (!head) head = tail = it;
            else { tail->next = it; tail = it; }

            if (current_token.type == TOKEN_COMMA) {
                free_token(current_token);
                current_token = next_token();
                continue;
            }
            break;
        }

        if (paren) {
            if (current_token.type == TOKEN_RPAREN) {
                free_token(current_token);
                current_token = next_token();
            } else {
                /* missing closing paren - tolerate */
            }
        }

        Node* pnode = make_node(NODE_PRINT);
        pnode->args = head;
        return pnode;
    }

    /* assign IDENT = expr */
    if (current_token.type == TOKEN_ASSIGN_KW) {
        free_token(current_token);
        current_token = next_token();
        if (current_token.type != TOKEN_IDENTIFIER) return NULL;
        char* name = strdup(current_token.text);
        free_token(current_token);
        current_token = next_token();

        if (current_token.type != TOKEN_ASSIGN) {
            free(name);
            return NULL;
        }
        free_token(current_token);
        current_token = next_token();

        Node* expr = parse_expr();
        Node* n = make_node(NODE_ASSIGN);
        n->name = name;
        n->right = expr;
        return n;
    }

    /* otherwise an expression */
    return parse_expr();
}

/* parse() público */
Node* parse(void) {
    current_token = next_token();
    Node* s = parse_statement_internal();
    /* free leftover token if any (not strictly necessary) */
    return s;
}
