#include "token.h"
#include <stdbool.h>

typedef struct {
    bool isEOF;
    char *input;
    int curPos;
    int readPos;
    int inputLen;
} Lexer;

Lexer *newLexer(char *input);
Token *lexer_next(Lexer *l);
void lexer_advance(Lexer *l);
Token *newToken(enum TokenType type, char *input, int start, int length);
void lexer_whitespace(Lexer *l);
char lexer_peek(Lexer *l);
