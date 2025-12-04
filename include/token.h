
#include <stdbool.h>
enum TokenType {
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  BANG,
  MODULO,
  EXPONENT,
  COMMA,
  DOT,

  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  LSQUARE,
  RSQUARE,

  CONCAT,
  ELLIPSIS,

  IDENTIFIER,
  INTEGER,
  FLOAT,

  IF,
  ELSE,
  RETURN,
  FN,
  END,
  LET,
  NOT,
  AND,
  OR,
  THEN,

  NIL,
  TRUE,
  FALSE,

  STRING_DOUBLE,
  STRING_SINGLE,
  STRING_MULTILINE,

  TYPE,

  EQ,
  NEQ,
  LTE,
  GTE,

  LT,
  GT,
  ASSIGN,

  TOKEN_EOF,
  TOKEN_INVALID,
};
typedef struct {
  enum TokenType Type;

  char *value;
} Token;
