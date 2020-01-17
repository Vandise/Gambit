#include "modules/tokens/token_module.h"

RESERVED_WORD_TOKEN rw1[] = {
  { NULL, 0 }
};

RESERVED_WORD_TOKEN rw2[] = {
  { NULL, 0 }
};

RESERVED_WORD_TOKEN rw3[] = {
  { NULL, 0 }
};

RESERVED_WORD_TOKEN rw4[] = {
  { NULL, 0 }
};

RESERVED_WORD_TOKEN rw5[] = {
  { NULL, 0 }
};

RESERVED_WORD_TOKEN rw6[] = {
  { NULL, 0 }
};

RESERVED_WORD_TOKEN *rw_table[] = { NULL, rw1, rw2, rw3, rw4, rw5, rw6 };

// ============================
//        Implementation
// ============================

static BOOLEAN string_is_reserved_word(const char* word_string) {
  log_trace("TokenModule::string_is_reserved_word");
  log_info("TokenModule::string_is_reserved_word (%s)", word_string);

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

static TOKEN_CODE get_token_code(const char* word_string) {
  log_trace("TokenModule::get_token_code");
  log_info("TokenModule::get_token_code (%s)", word_string);

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

static BOOLEAN token_in_list(TOKEN_CODE token, TOKEN_CODE token_list[]) {
  log_trace("TokenModule::token_in_list");

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

static void init_token_array(TokenArray *a, size_t initialSize) {
  log_trace("TokenModule::init_token_array");

  a->array = (Token*)(__MALLOC__(initialSize * sizeof(Token)));
  a->used = 0;
  a->size = initialSize;
}

static void insert_token_array(TokenArray *a, Token token) {
  log_trace("TokenModule::insert_token_array");
  log_info("TokenModule::insert_token_array (%d)", (int)token.code);

  if (a->used == a->size) {
    log_trace("TokenModule::insert_token_array (realloc)");
    // 3 / 2 gives you 50% worst and 25% typical
    // + 8 for arrays which are reasonably small don't end up doing too many copie
    a->size = (a->size * 3) / 2 + 8;
    a->array = (Token*)(__REALLOC__(a->array, a->size * sizeof(Token)));
  }
  a->array[a->used++] = token;
}

static void free_token_array(TokenArray *a) {
  log_trace("TokenModule::free_token_array");

  __FREE__(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

// ============================
//            Module
// ============================

const struct token_module TokenModule = {
  .version = GAMBIT_TOKEN_MODULE_VERSION,

  .string_is_reserved_word = string_is_reserved_word,
  .get_token_code = get_token_code,
  .token_in_list = token_in_list,
  .init_token_array = init_token_array,
  .insert_token_array = insert_token_array,
  .free_token_array = free_token_array
};