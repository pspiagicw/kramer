#include "parser.h"
#include <stdlib.h>

Parser *newParser(Lexer *l) {
    Parser *p = malloc(sizeof(Parser));

    p->lexer = l;

    return p;
}

AST *parser_parse(Parser *p) { return malloc(sizeof(AST)); }

char *ast_to_string(AST *ast) { return ""; }
