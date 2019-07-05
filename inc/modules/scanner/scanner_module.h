#ifndef __GAMBIT_SCANNER_MODULEH
#define __GAMBIT_SCANNER_MODULEH 1

#include "shared/scanner.h"

#define __GAMBIT_SCANNER_TOKENLIST_SIZE 100
#define CHAR_CODE(scanner) scanner->char_table[scanner->current_char]

typedef struct GambitScannerToken {
  char word_string[MAX_TOKEN_STRING_LENGTH];
  char token_string[MAX_TOKEN_STRING_LENGTH];
  char *tokenp;
  TOKEN_CODE token;
  LITERAL literal;
} ScannerToken;

typedef struct GambitScanner {
  unsigned int line_number;
  unsigned int level;
  unsigned int buffer_offset;
  unsigned int digit_count;

  CHAR_CODE char_table[256];

  BOOLEAN count_error;
  BOOLEAN errored;

  char source_buffer[MAX_SOURCE_LINE_LENGTH];
  char source_name[MAX_FILE_NAME_LENGTH];
  FILE *source_file;

  char *source_bufferp;
  char current_char;

  ScannerToken current_token;
  TokenArray tokens;
} Scanner;

void populate_char_table(Scanner* scanner);
BOOLEAN open_source_file(Scanner* scanner);
void get_character(Scanner* scanner);
BOOLEAN get_source_line(Scanner* scanner);
void skip_blanks(Scanner* scanner);
void skip_comment(Scanner* scanner);
void get_token(Scanner* scanner);
void get_special(Scanner* scanner);
void get_word(Scanner* scanner, BOOLEAN is_constant);
void get_string(Scanner* scanner);
void commit_token(Scanner* scanner);

#endif
