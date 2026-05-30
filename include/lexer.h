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
void lexer_retreat(Lexer *l);

Token *newToken(enum TokenType type, char *input, int start, int length);
Token *newTokenFromValue(enum TokenType type, char *value);

void lexer_whitespace(Lexer *l);
char lexer_peek(Lexer *l);

char *lexer_identifier(Lexer *l);
char *lexer_number(Lexer *l);
char *lexer_double_string(Lexer *l);
char *lexer_single_string(Lexer *l);
char *lexer_multiline_string(Lexer *l);

enum TokenType predict_type(char *value);

bool is_letter(char);
// Modification of is_letter, that includes '-'.
bool is_iname(char);
bool is_digit(char);

void lexer_comment(Lexer *l);
