#include "token.h"

#define X(name) #name,
static const char *token_names[] = {TOKEN_TYPES};
#undef X

const char *token_type_name(enum TokenType t) { return token_names[t]; }
