#ifndef __GAMBIT_TOKEN
#define __GAMBIT_TOKEN 1

#include "common.h"
#include <string.h>

#define __GAMBIT_TOKEN_VERSION      1
#define MIN_RESERVED_WORD_LENGTH    2
#define MAX_RESERVED_WORD_LENGTH    6
#define STATEMENT_START_SIZE        1
#define STATEMENT_END_SIZE          1

#define EOF_CHAR '\x7f'

typedef enum {
    NO_TOKEN, T_END_OF_FILE, T_NEWLINE, T_MODULE, T_DO, T_END, T_IDENTIFIER
} TOKEN;

typedef enum {
    LETTER, UPPERCASE_LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE, NEWLINE
} CHAR_CODE;

// reserved words
typedef struct {
    char       *string;
    TOKEN token_code;
} RESERVED_WORD_TOKEN;

typedef struct token_module token_module;

struct token_module {
  uint8_t version;
  BOOLEAN initialized;

  void (*initialize)(token_module*);
  BOOLEAN (*string_is_reserved_word)(const char* word_string);
  TOKEN (*get_token_code)(const char* word_string);
};

extern token_module TokenModule;

#endif