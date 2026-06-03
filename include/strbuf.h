#pragma once

typedef struct {
    char *buf;
    int len;
    int cap;
} StrBuf;

StrBuf *strbuf_new();
void strbuf_append(StrBuf *sb, const char *str);
char *strbuf_done(StrBuf *sb);
