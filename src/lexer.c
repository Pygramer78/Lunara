#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"
#include <stdio.h>

static const char* src = NULL;
static int pos = 0;

void lexer_init(const char* source) {
    src = source ? source : "";
    pos = 0;
}

Token make_token(TokenType type, const char* text) {
    Token t;
    t.type = type;
    t.text = (text && text[0] != '\0') ? strdup(text) : strdup("");
    t.value = 0.0;
    return t;
}

Token next_token() {
    while (src[pos] != '\0') {
        char c = src[pos];

        /* whitespace */
        if (isspace((unsigned char)c)) { pos++; continue; }

        /* numbers (integers and floats) */
        if (isdigit((unsigned char)c)) {
            char buf[128];
            int i = 0;
            while (isdigit((unsigned char)src[pos]) || src[pos] == '.') {
                if (i < (int)sizeof(buf)-1) buf[i++] = src[pos];
                pos++;
            }
            buf[i] = '\0';
            Token t = make_token(TOKEN_NUMBER, buf);
            t.value = atof(buf);
            return t;
        }

        /* strings "..." */
        if (c == '"') {
            pos++; /* skip " */
            char buf[512];
            int i = 0;
            while (src[pos] != '\0' && src[pos] != '"') {
                if (src[pos] == '\\' && src[pos+1] != '\0') {
                    /* escape simple sequences: \" \\ \n \t */
                    pos++;
                    char esc = src[pos];
                    if (esc == 'n') buf[i++] = '\n';
                    else if (esc == 't') buf[i++] = '\t';
                    else buf[i++] = esc;
                    pos++;
                    continue;
                }
                if (i < (int)sizeof(buf)-1) buf[i++] = src[pos++];
                else pos++;
            }
            buf[i] = '\0';
            if (src[pos] == '"') pos++;
            return make_token(TOKEN_STRING, buf);
        }

        /* char literal 'a' (simple) */
        if (c == '\'') {
            pos++;
            char ch = '\0';
            if (src[pos] == '\\' && src[pos+1] != '\0') {
                pos++;
                char esc = src[pos++];
                if (esc == 'n') ch = '\n';
                else if (esc == 't') ch = '\t';
                else ch = esc;
            } else {
                ch = src[pos++];
            }
            if (src[pos] == '\'') pos++;
            char s[2] = { ch, '\0' };
            return make_token(TOKEN_CHAR, s);
        }

        /* identifiers / keywords */
        if (isalpha((unsigned char)c) || c == '_') {
            char buf[128];
            int i = 0;
            while (isalnum((unsigned char)src[pos]) || src[pos] == '_') {
                if (i < (int)sizeof(buf)-1) buf[i++] = src[pos];
                pos++;
            }
            buf[i] = '\0';
            if (strcmp(buf, "assign") == 0) return make_token(TOKEN_ASSIGN_KW, buf);
            if (strcmp(buf, "print") == 0) return make_token(TOKEN_PRINT, buf);
            return make_token(TOKEN_IDENTIFIER, buf);
        }

        /* single-char tokens */
        switch (c) {
            case '=': pos++; return make_token(TOKEN_ASSIGN, "=");
            case ',': pos++; return make_token(TOKEN_COMMA, ",");
            case '+': pos++; return make_token(TOKEN_PLUS, "+");
            case '-': pos++; return make_token(TOKEN_MINUS, "-");
            case '*': pos++; return make_token(TOKEN_STAR, "*");
            case '/': pos++; return make_token(TOKEN_SLASH, "/");
            case '(': pos++; return make_token(TOKEN_LPAREN, "(");
            case ')': pos++; return make_token(TOKEN_RPAREN, ")");
            default: {
                char s[2] = { c, '\0' };
                pos++;
                return make_token(TOKEN_UNKNOWN, s);
            }
        }
    }

    return make_token(TOKEN_EOF, "");
}

void free_token(Token t) {
    if (t.text) free(t.text);
}


