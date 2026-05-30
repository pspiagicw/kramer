
#include <stdbool.h>

/* X-macro list: define each token type once, used to generate both the enum
   and the name lookup array. */
#define TOKEN_TYPES                                                            \
    X(PLUS)                                                                    \
    X(MINUS)                                                                   \
    X(MULTIPLY)                                                                \
    X(DIVIDE)                                                                  \
    X(BANG)                                                                    \
    X(MODULO)                                                                  \
    X(EXPONENT)                                                                \
    X(COMMA)                                                                   \
    X(DOT)                                                                     \
    X(LPAREN)                                                                  \
    X(RPAREN)                                                                  \
    X(LBRACE)                                                                  \
    X(RBRACE)                                                                  \
    X(LSQUARE)                                                                 \
    X(RSQUARE)                                                                 \
    X(CONCAT)                                                                  \
    X(ELLIPSIS)                                                                \
    X(IDENTIFIER)                                                              \
    X(INTEGER)                                                                 \
    X(FLOAT)                                                                   \
    X(IF)                                                                      \
    X(ELSE)                                                                    \
    X(RETURN)                                                                  \
    X(FN)                                                                      \
    X(END)                                                                     \
    X(LET)                                                                     \
    X(NOT)                                                                     \
    X(AND)                                                                     \
    X(OR)                                                                      \
    X(THEN)                                                                    \
    X(NIL)                                                                     \
    X(TRUE)                                                                    \
    X(FALSE)                                                                   \
    X(STRING_DOUBLE)                                                           \
    X(STRING_SINGLE)                                                           \
    X(STRING_MULTILINE)                                                        \
    X(TYPE)                                                                    \
    X(EQ)                                                                      \
    X(NEQ)                                                                     \
    X(LTE)                                                                     \
    X(GTE)                                                                     \
    X(LT)                                                                      \
    X(GT)                                                                      \
    X(ASSIGN)                                                                  \
    X(QUOTE)                                                                   \
    X(TOKEN_EOF)                                                               \
    X(TOKEN_INVALID)

#define X(name) name,
enum TokenType { TOKEN_TYPES };
#undef X

typedef struct {
    enum TokenType Type;
    char *Value;
} Token;

const char *token_type_name(enum TokenType t);
