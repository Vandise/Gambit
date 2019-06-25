#include "shared/tokens.h"

static RESERVED_WORD_TOKEN rw2[] = {
  {"do", T_DO }, { NULL, 0 }
};

static RESERVED_WORD_TOKEN rw3[] = {
  {"end", T_END }, { NULL, 0 }
};

static RESERVED_WORD_TOKEN rw4[] = {
  { NULL, 0 }
};

static RESERVED_WORD_TOKEN rw5[] = {
  { NULL, 0 }
};

static RESERVED_WORD_TOKEN rw6[] = {
  {"module", T_MODULE }, { NULL, 0 }
};

static RESERVED_WORD_TOKEN *rw_table[] = { NULL, NULL, rw2, rw3, rw4, rw5, rw6 };

static TOKEN statement_start_list[] = { 0 };
static TOKEN statement_end_list[] = { 0 };

static void initialize(token_module *self) {
  self->initialized = TRUE;
}

static BOOLEAN string_is_reserved_word(const char* word_string) {
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

static TOKEN get_token_code(const char* word_string) {
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

token_module TokenModule = {
  .version = __GAMBIT_TOKEN_VERSION,
  .initialized = FALSE,

  .string_is_reserved_word = string_is_reserved_word,
  .get_token_code = get_token_code,
  .initialize = initialize,
};