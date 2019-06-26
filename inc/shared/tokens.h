#ifndef __GAMBIT_SCANNER_TOKENSH
#define __GAMBIT_SCANNER_TOKENSH 1

#include "common.h"

#define EOF_CHAR '\x7f'

typedef enum {
  INTEGER_LIT, REAL_LIT, STRING_LIT,
} LITERAL_TYPE;

typedef enum {
    LETTER, UPPERCASE_LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE, NEWLINE
} CHAR_CODE;

typedef enum {
  NO_TOKEN
} TOKEN_CODE;

typedef struct {
  LITERAL_TYPE type;
  union {
    int   integer;
    float real;
    char  string[MAX_SOURCE_LINE_LENGTH];
  } value;
} LITERAL;

typedef struct {
  char *string;
  TOKEN_CODE token_code;
} RESERVED_WORD_TOKEN;

typedef struct {
  unsigned int line_number;
  unsigned int level;

  char source_name[MAX_FILE_NAME_LENGTH];
  char word_string[MAX_TOKEN_STRING_LENGTH];
  char token_string[MAX_TOKEN_STRING_LENGTH];

  TOKEN_CODE code;
  LITERAL literal;
} Token;

#endif
