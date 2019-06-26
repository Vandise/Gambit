#include "shared/token_array.h"

void init_token_array(TokenArray *a, size_t initialSize) {
  a->array = (Token*)(__MALLOC__(initialSize * sizeof(Token)));
  a->used = 0;
  a->size = initialSize;
}

void insert_token_array(TokenArray *a, Token token) {
  if (a->used == a->size) {
    // 3 / 2 gives you 50% worst and 25% typical
    // + 8 for arrays which are reasonably small don't end up doing too many copie
    a->size = (a->size * 3) / 2 + 8;
    a->array = (Token*)(__REALLOC__(a->array, a->size * sizeof(Token)));
  }
  a->array[a->used++] = token;
}

void free_token_array(TokenArray *a) {
  __FREE__(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}