#include "modules/scanner/scanner_module.h"

// ============================
//          Forwards
// ============================

static Scanner* init(const char *file_name);
static BOOLEAN open_source_file(Scanner *scanner);
static BOOLEAN get_source_line(Scanner* scanner);
static void populate_char_table(Scanner *scanner);
static void get_character(Scanner *scanner);
static void skip_comment(Scanner* scanner);
static void skip_blanks(Scanner* scanner);
static void close(Scanner *scanner);

// ============================
//        Implementation
// ============================

static Scanner* init(const char *file_name) {
  errno = 0;

  log_trace("Scanner::init %s", file_name);

  Scanner* scanner = __MALLOC__(sizeof(Scanner));
  strcpy(scanner->source_name, file_name);
  strcpy(scanner->source_buffer, "");

  scanner->current_char = '\0';
  scanner->source_bufferp = NULL;
  scanner->line_number = 0;
  scanner->level = 0;
  scanner->buffer_offset = 0;
  scanner->errored = FALSE;

  populate_char_table(scanner);

  if (open_source_file(scanner) == FALSE) {
    log_error("Scanner failed to open file %s", file_name);
    log_error("Scanner errored with errno %d", errno);
    scanner->errored = TRUE;
    scanner->error_code = SCANNER_FAILED_TO_OPEN_FILE;
  }

  return scanner;
}

static void populate_char_table(Scanner *scanner) {
  log_trace("Scanner::populate_char_table");

  int ch;

  for (ch = 0;   ch < 256;  ++ch) scanner->char_table[ch] = SPECIAL;
  for (ch = '0'; ch <= '9'; ++ch) scanner->char_table[ch] = DIGIT;
  for (ch = 'A'; ch <= 'Z'; ++ch) scanner->char_table[ch] = UPPERCASE_LETTER;
  for (ch = 'a'; ch <= 'z'; ++ch) scanner->char_table[ch] = LETTER;
  scanner->char_table['\''] = QUOTE;
  scanner->char_table['\n'] = NEWLINE;
  scanner->char_table['_'] = UNDERSCORE;
  scanner->char_table[EOF_CHAR] = EOF_CODE;

}

static BOOLEAN open_source_file(Scanner *scanner) {
  log_trace("Scanner::open_source_file");

  scanner->source_file = fopen(scanner->source_name, "r");

  if (errno != 0) {
    return FALSE;
  }

  scanner->source_bufferp = scanner->source_buffer;
  get_character(scanner);

  return TRUE;
}

static BOOLEAN get_source_line(Scanner* scanner) {
  log_trace("Scanner::get_source_line");

  if ( scanner != NULL && fgets(scanner->source_buffer, 1024, scanner->source_file) != NULL ) {
    scanner->line_number += 1;

    log_info(scanner->source_buffer);

    return TRUE;
  }

  return FALSE;
}

static void get_character(Scanner *scanner) {
  log_trace("Scanner::get_character");

  if (*(scanner->source_bufferp) == '\0') {
    if (!get_source_line(scanner)) {
      log_trace("Scanner::get_character setting current char to EOF_CHAR");
      scanner->current_char = EOF_CHAR;
      return;
    }

    scanner->source_bufferp = scanner->source_buffer;
    scanner->buffer_offset = 0;
  }

  scanner->current_char = *(scanner->source_bufferp)++;

  log_info("Scanner::get_character - %c", scanner->current_char);

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

static void skip_comment(Scanner* scanner) {
  log_trace("Scanner::skip_comment");

  do {
    get_character(scanner);
  } while ( (scanner->current_char != NEWLINE_CHAR) && (scanner->current_char != EOF_CHAR) );
}

static void skip_blanks(Scanner* scanner) {
  while(scanner->current_char == ' ') {
    get_character(scanner);
  }
}

static void close(Scanner *scanner) {
  log_trace("Scanner::close");

  if (scanner->source_file != NULL) {
    fclose(scanner->source_file);
  }

  __FREE__(scanner);
}

// ============================
//            Module
// ============================

const struct scanner_module ScannerModule = {
  .version = GAMBIT_SCANNER_VERSION,
  .init = init,
  .open_source_file = open_source_file,
  .get_character = get_character,
  .skip_comment = skip_comment,
  .get_source_line = get_source_line,
  .skip_blanks = skip_blanks,
  .close = close
};