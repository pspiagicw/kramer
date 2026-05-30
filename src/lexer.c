#include "lexer.h"
#include <stdlib.h>
#include <string.h>

#define STRCASE(str, lit, ret) (strcmp((str), (lit)) == 0 ? (ret) : 0)

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

    if (l->readPos > l->inputLen) {
        l->isEOF = true;
    }
}
void lexer_retreat(Lexer *l) {
    l->curPos -= 1;
    l->readPos -= 1;
    l->isEOF = false;

    if (l->readPos > l->inputLen) {
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
    case '`':
        tok = newToken(QUOTE, l->input, l->curPos, 1);
        break;
    case '"':
        char *double_string = lexer_double_string(l);
        tok = newTokenFromValue(STRING_DOUBLE, double_string);
        break;
    case '\'':
        char *single_string = lexer_single_string(l);
        tok = newTokenFromValue(STRING_SINGLE, single_string);
        break;
    case '=':
        if (lexer_peek(l) == '=') {
            tok = newToken(EQ, l->input, l->curPos, 2);
            lexer_advance(l);
        } else {
            tok = newToken(ASSIGN, l->input, l->curPos, 1);
        }
        break;
    case '+':
        tok = newToken(PLUS, l->input, l->curPos, 1);
        break;
    case '-':
        if (lexer_peek(l) == '-') {
            lexer_comment(l);
            return lexer_next(l);
        }
        tok = newToken(MINUS, l->input, l->curPos, 1);
        break;
    case '*':
        tok = newToken(MULTIPLY, l->input, l->curPos, 1);
        break;
    case '/':
        tok = newToken(DIVIDE, l->input, l->curPos, 1);
        break;
    case '<':
        if (lexer_peek(l) == '=') {
            tok = newToken(LTE, l->input, l->curPos, 2);
            lexer_advance(l);
        } else {
            tok = newToken(LT, l->input, l->curPos, 1);
        }
        break;
    case '>':
        if (lexer_peek(l) == '=') {
            tok = newToken(GTE, l->input, l->curPos, 2);
            lexer_advance(l);
        } else {
            tok = newToken(GT, l->input, l->curPos, 1);
        }
        break;
    case '(':
        tok = newToken(LPAREN, l->input, l->curPos, 1);
        break;
    case ')':
        tok = newToken(RPAREN, l->input, l->curPos, 1);
        break;
    case '{':
        tok = newToken(LBRACE, l->input, l->curPos, 1);
        break;
    case '}':
        tok = newToken(RBRACE, l->input, l->curPos, 1);
        break;
    case '[':
        if (lexer_peek(l) == '[') {
            char *multiline_string = lexer_multiline_string(l);
            tok = newTokenFromValue(STRING_MULTILINE, multiline_string);
        } else {
            tok = newToken(LSQUARE, l->input, l->curPos, 1);
        }
        break;
    case ']':
        tok = newToken(RSQUARE, l->input, l->curPos, 1);
        break;
    case '!':
        if (lexer_peek(l) == '=') {
            tok = newToken(NEQ, l->input, l->curPos, 2);
            lexer_advance(l);
        } else {
            tok = newToken(BANG, l->input, l->curPos, 1);
        }
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
        if (is_letter(curChar)) {
            char *value = lexer_identifier(l);
            enum TokenType token_type = predict_type(value);
            tok = newTokenFromValue(token_type, value);
            lexer_retreat(l);

        } else if (is_digit(curChar)) {
            char *value = lexer_number(l);
            tok = newTokenFromValue(INTEGER, value);
            lexer_retreat(l);
        } else {
            tok = newToken(TOKEN_INVALID, "", 0, 0);
        }
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
    t->Value = dest;

    return t;
}

Token *newTokenFromValue(enum TokenType type, char *value) {
    Token *t = malloc(sizeof(Token));

    t->Type = type;
    t->Value = value;

    return t;
}

char lexer_peek(Lexer *l) {
    if (l->readPos >= l->inputLen) {
        return '\0';
    }
    char peek = l->input[l->readPos];
    return peek;
}

char *lexer_identifier(Lexer *l) {
    int start = l->curPos;

    while (!l->isEOF && is_iname(l->input[l->curPos])) {
        lexer_advance(l);
    }

    int end = l->curPos;

    int length = (end - start) + 1;

    char *dest = malloc(sizeof(char) * length);
    strncpy(dest, l->input + start, length - 1);
    dest[length] = '\0';

    return dest;
}

char *lexer_number(Lexer *l) {
    int start = l->curPos;

    while (!l->isEOF && is_digit(l->input[l->curPos])) {
        lexer_advance(l);
    }

    int end = l->curPos;
    int length = (end - start) + 1;

    char *dest = malloc(sizeof(char) * length);
    strncpy(dest, l->input + start, length - 1);
    dest[length] = '\0';

    return dest;
}

char *lexer_double_string(Lexer *l) {
    // Skip over the quote.
    lexer_advance(l);

    int start = l->curPos;

    // TOOD: Add checks for \n.
    while (!l->isEOF && l->input[l->curPos] != '"') {
        lexer_advance(l);
    }

    int end = l->curPos;

    // No +1, cause the end index is and end-quote, the string will end 1
    // character before.
    int length = (end - start) + 1;

    char *dest = malloc(sizeof(char) * length);
    strncpy(dest, l->input + start, length - 1);
    dest[length] = '\0';

    return dest;
}

void lexer_comment(Lexer *l) {
    // Skip over both - .
    lexer_advance(l);
    lexer_advance(l);

    // Check for [[, if yes run lexer_multiline_string and simply skip over the
    // comment.
    if (!l->isEOF && l->input[l->curPos] == '[' && lexer_peek(l) == '[') {
        lexer_multiline_string(l);
        // Skip over the second ]
        lexer_advance(l);
    } else {
        // It's a single-line commenta and go on till the newline.
        while (!l->isEOF && l->input[l->curPos] != '\n') {
            lexer_advance(l);
        }
    }
}

char *lexer_single_string(Lexer *l) {
    // Skip over the quote.
    lexer_advance(l);

    int start = l->curPos;

    // TODO: Add checks for \n.
    while (!l->isEOF && l->input[l->curPos] != '\'') {
        lexer_advance(l);
    }

    int end = l->curPos;

    int length = (end - start) + 1;

    char *dest = malloc(sizeof(char) * length);
    strncpy(dest, l->input + start, length - 1);
    dest[length] = '\0';

    return dest;
}

char *lexer_multiline_string(Lexer *l) {

    // Skip over the [[
    lexer_advance(l);
    lexer_advance(l);

    int start = l->curPos;

    while (!l->isEOF) {
        if (l->input[l->curPos] == ']' && lexer_peek(l) == ']') {
            // Move over current ]. The second ] will be covered by end of
            // lexer_next().
            lexer_advance(l);
            break;
        } else {
            lexer_advance(l);
        }
    }

    int end = l->curPos - 1;

    int length = (end - start) + 1;
    char *dest = malloc(sizeof(char) * length);
    strncpy(dest, l->input + start, length - 1);
    dest[length] = '\n';

    return dest;
}

enum TokenType predict_type(char *value) {
    return STRCASE(value, "if", IF) ?:
    STRCASE(value, "else", ELSE) ?:
    STRCASE(value, "return", RETURN) ?:
    STRCASE(value, "fn", FN) ?:
    STRCASE(value, "end", END) ?:
    STRCASE(value, "let", LET) ?:
    STRCASE(value, "not", NOT) ?:
    STRCASE(value, "and", AND) ?:
    STRCASE(value, "or", OR) ?:
    STRCASE(value, "then", THEN) ?:
    STRCASE(value, "nil", NIL) ?:
    STRCASE(value, "true", TRUE) ?:
    STRCASE(value, "false", FALSE) ?:
    STRCASE(value, "int", INTEGER) ?:
    STRCASE(value, "float", FLOAT) ?:
    IDENTIFIER;
}

bool is_letter(char s) {
    if (s == '_') {
        return true;
    }
    if ('a' <= s && 'z' >= s) {
        return true;
    } else if ('A' <= s && 'Z' >= s) {
        return true;
    }
    return false;
}
bool is_iname(char s) { return is_letter(s) || s == '-'; }

bool is_digit(char s) {
    if ('0' <= s && '9' >= s) {
        return true;
    }
    return false;
}
