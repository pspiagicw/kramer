#include "strbuf.h"
#include <stdlib.h>
#include <string.h>

#define STRBUF_INIT_CAP 64

StrBuf *strbuf_new() {
    StrBuf *sb = malloc(sizeof(StrBuf));
    sb->buf = malloc(STRBUF_INIT_CAP);
    sb->buf[0] = '\0';
    sb->len = 0;
    sb->cap = STRBUF_INIT_CAP;
    return sb;
}

void strbuf_append(StrBuf *sb, const char *str) {
    int slen = strlen(str);
    while (sb->len + slen + 1 > sb->cap) {
        sb->cap *= 2;
        sb->buf = realloc(sb->buf, sb->cap);
    }
    memcpy(sb->buf + sb->len, str, slen + 1);
    sb->len += slen;
}

char *strbuf_done(StrBuf *sb) {
    char *result = sb->buf;
    free(sb);
    return result;
}
