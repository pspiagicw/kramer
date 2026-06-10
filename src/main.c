#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: kramer <file>\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "error: cannot open '%s'\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char *src = malloc(len + 1);
    fread(src, 1, len, f);
    src[len] = '\0';
    fclose(f);

    Lexer *l = newLexer(src);
    Parser *p = newParser(l);
    parser_parse(p);

    int numErrors = p->numErrors;
    if (numErrors > 0) {
        Error *errors = parser_errors(p);
        for (int i = 0; i < numErrors; i++) {
            fprintf(stderr, "error: %s\n", errors[i].Value);
        }
        return 1;
    }

    return 0;
}
