#include "token.h"
#include <stdbool.h>

typedef struct {
  bool isEOF;
  char *input;
} Lexer;

Lexer *newLexer(char *input);
Token *nextToken(Lexer *l);
