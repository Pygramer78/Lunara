#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN_KW,   // palabra clave: assign
    TOKEN_ASSIGN,      // símbolo "="
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* text;   // texto del token
    double value; // si es un número
} Token;

void lexer_init(const char* source);
Token next_token(void);
void free_token(Token t);
Token make_token(TokenType type, const char* text);

#endif

