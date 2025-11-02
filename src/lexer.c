#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"

static const char* src;
static int pos = 0;

void lexer_init(const char* source) {
    src = source;
    pos = 0;
}

Token make_token(TokenType type, const char* text) {
    Token t;
    t.type = type;
    t.text = strdup(text);
    t.value = 0.0;
    return t;
}

Token next_token() {
    while (src[pos] != '\0') {
        char c = src[pos];

        // Ignorar espacios
        if (isspace(c)) {
            pos++;
            continue;
        }

        // Números
        if (isdigit(c)) {
            char buf[64];
            int i = 0;
            while (isdigit(src[pos]) || src[pos] == '.') {
                buf[i++] = src[pos++];
            }
            buf[i] = '\0';
            Token t = make_token(TOKEN_NUMBER, buf);
            t.value = atof(buf);
            return t;
        }

        // Palabras reservadas o identificadores
        if (isalpha(c)) {
            char buf[64];
            int i = 0;
            while (isalnum(src[pos])) {
                buf[i++] = src[pos++];
            }
            buf[i] = '\0';

            if (strcmp(buf, "assign") == 0)
                return make_token(TOKEN_ASSIGN_KW, buf);

            return make_token(TOKEN_IDENTIFIER, buf);
        }

        // Símbolos
        switch (c) {
            case '+': pos++; return make_token(TOKEN_PLUS, "+");
            case '-': pos++; return make_token(TOKEN_MINUS, "-");
            case '*': pos++; return make_token(TOKEN_STAR, "*");
            case '/': pos++; return make_token(TOKEN_SLASH, "/");
            case '=': pos++; return make_token(TOKEN_ASSIGN, "=");
            case '(': pos++; return make_token(TOKEN_LPAREN, "(");
            case ')': pos++; return make_token(TOKEN_RPAREN, ")");
            default: {
                char str[2] = { c, '\0' };
                pos++;
                return make_token(TOKEN_UNKNOWN, str);
            }
        }
    }

    return make_token(TOKEN_EOF, "");
}

void free_token(Token t) {
    free(t.text);
}

