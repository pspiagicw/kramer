
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

  TOKEN_EOF,
};
typedef struct {
  enum TokenType Type;

  char *value;
} Token;
