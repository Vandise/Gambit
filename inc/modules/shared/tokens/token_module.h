#ifndef __GAMBIT_TOKENSH
#define __GAMBIT_TOKENSH 1

// ============================
//           Includes
// ============================

#include <float.h>
#include "common.h"

// ============================
//           Macros
// ============================

#define GAMBIT_TOKEN_MODULE_VERSION 100
#define MIN_RESERVED_WORD_LENGTH 1
#define MAX_RESERVED_WORD_LENGTH 6

// FLT_MIN, FLT_MAX, FLT_DIG
// DBL_MIN, DBL_MAX, DBL_DIG

#define I_32_LOWER -2147483648
#define I_32_UPPER  2147483647
#define I_64_LOWER -9223372036854775807
#define I_64_UPPER  9223372036854775806

// ============================
//       Structs & Types
// ============================

typedef enum {
  I_32LIT, I_64LIT, F_32LIT, F_64LIT, STRING_LIT,
} LITERAL_TYPE;

typedef enum {
  NO_TOKEN, T_ERROR, T_NEWLINE, T_END_OF_FILE,
  T_CONSTANT, T_IDENTIFIER, T_STRING, T_NUMERIC
} TOKEN_CODE;

typedef struct scanner_reserved_word_token {
  char *string;
  TOKEN_CODE token_code;
} RESERVED_WORD_TOKEN;

typedef struct scanner_literal {
  LITERAL_TYPE type;
  size_t size;
  union {
    int i32;
    long int i64;
    float f32;
    double f64;
    char  string[1024];
  } value;
} LITERAL;

typedef struct scanner_token {
  unsigned int line_number;
  unsigned int level;

  char source_name[64];
  char word_string[1024];
  char token_string[1024];

  TOKEN_CODE code;
  LITERAL literal;
} Token;

typedef struct token_array {
  Token *array;
  size_t used;
  size_t size;
} TokenArray;

struct token_module {
  const signed int version;

  BOOLEAN (*string_is_reserved_word)(const char*);
  TOKEN_CODE (*get_token_code)(const char*);
  BOOLEAN (*token_in_list)(TOKEN_CODE, TOKEN_CODE[]);

  void (*init_token_array)(TokenArray*, size_t);
  void (*insert_token_array)(TokenArray*, Token);
  void (*free_token_array)(TokenArray*);
};

// ============================
//          Externs
// ============================

extern const struct token_module TokenModule;

#endif