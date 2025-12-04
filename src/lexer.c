#include "lexer.h"
#include <stdlib.h>
#include <string.h>

Lexer *newLexer(char *input) {
  Lexer *l = malloc(sizeof(Lexer));
  l->input = input;
  l->isEOF = false;
  l->readPos = 0;
  l->curPos = -1;
  l->inputLen = strlen(input);

  lexer_advance(l);
  return l;
}

void lexer_advance(Lexer *l) {
  l->curPos = l->readPos;
  l->readPos += 1;

  if (l->readPos >= l->inputLen) {
    l->isEOF = true;
  }
}

void lexer_whitespace(Lexer *l) {
  char curChar = l->input[l->curPos];
  while (curChar == ' ' || curChar == '\t' || curChar == '\n') {
    lexer_advance(l);
    curChar = l->input[l->curPos];
  }
}

Token *lexer_next(Lexer *l) {
  Token *tok = newToken(TOKEN_INVALID, "", 0, 0);

  if (l->isEOF) {
    return newToken(TOKEN_EOF, "", 0, 0);
  }

  lexer_whitespace(l);

  char curChar = l->input[l->curPos];

  switch (curChar) {
  case '+':
    tok = newToken(PLUS, l->input, l->curPos, 1);
    break;
  case '-':
    tok = newToken(MINUS, l->input, l->curPos, 1);
    break;
  case '*':
    tok = newToken(MULTIPLY, l->input, l->curPos, 1);
    break;
  case '/':
    tok = newToken(DIVIDE, l->input, l->curPos, 1);
    break;
  case '!':
    tok = newToken(BANG, l->input, l->curPos, 1);
    break;
  case '%':
    tok = newToken(MODULO, l->input, l->curPos, 1);
    break;
  case '^':
    tok = newToken(EXPONENT, l->input, l->curPos, 1);
    break;
  case ',':
    tok = newToken(COMMA, l->input, l->curPos, 1);
    break;
  case '.':
    tok = newToken(DOT, l->input, l->curPos, 1);
    break;
  default:
    tok = newToken(TOKEN_INVALID, "", 0, 0);
  }

  lexer_advance(l);

  return tok;
}

Token *newToken(enum TokenType type, char *value, int start, int length) {
  Token *t = malloc(sizeof(Token));

  char *dest = malloc(sizeof(char) * (length + 1));

  strncpy(dest, value + start, length);
  dest[length] = '\0';
  t->Type = type;
  t->value = dest;

  return t;
}
