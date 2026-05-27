#include "ast.h"
#include "lexer.h"

typedef struct {
    Lexer *lexer;
} Parser;

Parser *newParser(Lexer *l);
AST *parser_parse(Parser *p);
char *ast_to_string(AST *ast);
