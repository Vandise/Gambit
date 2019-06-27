#include "modules/scanner/scanner_module.h"

Scanner* init_scanner(char *file_name) {
  errno = 0;
  Scanner* scanner = __MALLOC__(sizeof(Scanner));
  strcpy(scanner->source_name, file_name);
  strcpy(scanner->source_buffer, "");

  scanner->current_char = '\0';
  scanner->source_bufferp = NULL;

  scanner->current_token.tokenp = scanner->current_token.token_string;

  scanner->line_number = 0;
  scanner->level = 0;
  scanner->buffer_offset = 0;
  scanner->errored = FALSE;

  populate_char_table(scanner);

  if (open_source_file(scanner) == FALSE) {
    __FREE__(scanner);
    return NULL;
  }

  init_token_array(&(scanner->tokens), __GAMBIT_SCANNER_TOKENLIST_SIZE);

  return scanner;
}

void populate_char_table(Scanner *scanner) {
  int ch;

  for (ch = 0;   ch < 256;  ++ch) scanner->char_table[ch] = SPECIAL;
  for (ch = '0'; ch <= '9'; ++ch) scanner->char_table[ch] = DIGIT;
  for (ch = 'A'; ch <= 'Z'; ++ch) scanner->char_table[ch] = UPPERCASE_LETTER;
  for (ch = 'a'; ch <= 'z'; ++ch) scanner->char_table[ch] = LETTER;
  scanner->char_table['\''] = QUOTE;
  scanner->char_table['\n'] = NEWLINE;
  scanner->char_table[EOF_CHAR] = EOF_CODE;
}

BOOLEAN open_source_file(Scanner *scanner) {
  scanner->source_file = fopen(scanner->source_name, "r");

  if (errno != 0) {
    return FALSE;
  }

  scanner->source_bufferp = scanner->source_buffer;
  get_character(scanner);

  return TRUE;
}

void get_character(Scanner *scanner) {
  if (*(scanner->source_bufferp) == '\0') {
    if (!get_source_line(scanner)) {
      scanner->current_char = EOF_CHAR;
      return;
    }

    scanner->source_bufferp = scanner->source_buffer;
    scanner->buffer_offset = 0;
  }

  scanner->current_char = *(scanner->source_bufferp)++;

  switch(scanner->current_char) {
    case COMMENT_CHAR:
      scanner->buffer_offset += 1;
      skip_comment(scanner);
      scanner->current_char = ' ';
      break;
    default:
      scanner->buffer_offset += 1;
  }

}

BOOLEAN get_source_line(Scanner* scanner) {
  if ( scanner != NULL && fgets(scanner->source_buffer, MAX_SOURCE_LINE_LENGTH, scanner->source_file) != NULL ) {
    scanner->line_number += 1;
    // todo: debug
    //  print_source_buffer(scanner);
    //
    return TRUE;
  }
  return FALSE;
}

void skip_comment(Scanner* scanner) {
  do {
    get_character(scanner);
  } while ( (scanner->current_char != NEWLINE_CHAR) && (scanner->current_char != EOF_CHAR) );
}

void skip_blanks(Scanner* scanner) {
  while(scanner->current_char == ' ') {
    get_character(scanner);
  }
}

void exit_scanner(Scanner *scanner) {
  if (scanner->source_file != NULL) {
    fclose(scanner->source_file);
  }
  free_token_array(&(scanner->tokens));
  __FREE__(scanner);
}