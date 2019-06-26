#ifndef __GAMBIT_TOKEN_ARRAYH
#define __GAMBIT_TOKEN_ARRAYH 1

#include "util/memory.h"
#include "shared/tokens.h"

typedef struct {
  Token *array;
  size_t used;
  size_t size;
} TokenArray;

void init_token_array(TokenArray *a, size_t initialSize);
void insert_token_array(TokenArray *a, Token token);
void free_token_array(TokenArray *a);

#endif