#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_PRINT,       // palabra clave print
    TOKEN_ASSIGN_KW,   // palabra clave assign
    TOKEN_ASSIGN,      // '='
    TOKEN_COMMA,
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
    char* text;   /* texto del token (ident, string literal, symbol) */
    double value; /* si es number */
} Token;

void lexer_init(const char* source);
Token next_token(void);
void free_token(Token t);
Token make_token(TokenType type, const char* text);

#endif

