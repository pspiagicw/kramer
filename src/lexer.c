#include "lexer.h"
#include <stdlib.h>

Lexer *newLexer(char *input) {
  Lexer *l = malloc(sizeof(Lexer));
  l->input = input;
  l->isEOF = false;
  return l;
}

Token *nextToken(Lexer *l) { return NULL; }
