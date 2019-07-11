#include "shared/tokens.h"

RESERVED_WORD_TOKEN rw3[] = {
  {"let", T_LET }, { NULL, 0 }
};

RESERVED_WORD_TOKEN *rw_table[] = { NULL, NULL, NULL, rw3 };

BOOLEAN string_is_reserved_word(const char* word_string) {
  int word_length = strlen(word_string);
  RESERVED_WORD_TOKEN *rwp;

  if (word_length >= MIN_RESERVED_WORD_LENGTH && word_length <= MAX_RESERVED_WORD_LENGTH) {
    for (rwp = rw_table[word_length]; rwp->string != NULL; ++rwp) {
      if (strcmp(word_string, rwp->string) == 0) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

TOKEN_CODE get_token_code(const char* word_string) {
  int word_length = strlen(word_string);
  RESERVED_WORD_TOKEN *rwp;

  if (word_length >= MIN_RESERVED_WORD_LENGTH && word_length <= MAX_RESERVED_WORD_LENGTH) {
    for (rwp = rw_table[word_length]; rwp->string != NULL; ++rwp) {
      if (strcmp(word_string, rwp->string) == 0) {
        return rwp->token_code;
      }
    }
  }
  return NO_TOKEN;
}

BOOLEAN token_in_list(TOKEN_CODE token, TOKEN_CODE token_list[]) {
  TOKEN_CODE *tokenp;

  if (token_list == NULL) {
    return FALSE;
  }

  for (tokenp = &token_list[0]; *tokenp; ++tokenp) {
    if (token == *tokenp) {
      return TRUE;
    }
  }

  return FALSE;
}