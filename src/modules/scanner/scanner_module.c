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
static void get_word(Scanner* scanner, BOOLEAN is_constant);
static void get_string(Scanner* scanner);
static void get_number(Scanner* scanner);
static void accumulate_value(Scanner *scanner, double *valuep);
static void get_special(Scanner *scanner);

// ============================
//        Implementation
// ============================

static Scanner* init(const char *file_name) {
  errno = 0;

  log_trace("Scanner::init %s", file_name);

  Scanner* scanner = __MALLOC__(sizeof(Scanner));
  strcpy(scanner->source_name, file_name);
  strcpy(scanner->source_buffer, "");

  scanner->current_token.tokenp = scanner->current_token.token_string;

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

static void get_word(Scanner* scanner, BOOLEAN is_constant) {
  log_trace("Scanner::get_word (%d)", is_constant);

  while(
    CHAR_CODE(scanner) == LETTER ||
    CHAR_CODE(scanner) == UPPERCASE_LETTER ||
    CHAR_CODE(scanner) == UNDERSCORE
  ) {
    *(scanner->current_token.tokenp)++ = scanner->current_char;
    get_character(scanner);
  }

  *(scanner->current_token.tokenp) = '\0';

  log_info("Scanner::get_word (%s)", scanner->current_token.token_string);

  if(!TokenModule.string_is_reserved_word(scanner->current_token.token_string)) {
    if (is_constant) {
      scanner->current_token.token = T_CONSTANT;
    } else {
      scanner->current_token.token = T_IDENTIFIER;
    }
  } else {
    scanner->current_token.token = TokenModule.get_token_code(scanner->current_token.token_string);
  }
}

static void get_string(Scanner* scanner) {
  log_trace("Scanner::get_string");

  char* literalp = scanner->current_token.literal.value.string;
  *(scanner->current_token.tokenp)++ = '\'';

  get_character(scanner);

  while(scanner->current_char != EOF_CHAR) {
    if(scanner->current_char == '\'') {
      *(scanner->current_token.tokenp)++ = scanner->current_char;
      get_character(scanner);
      if(scanner->current_char != '\'') { break; }
    }

    *(scanner->current_token.tokenp)++ = scanner->current_char;
    *literalp++ = scanner->current_char;
    get_character(scanner);
  }

  log_trace("Scanner::get_string (%s) (%d)",
    scanner->current_token.literal.value.string,
    strlen(scanner->current_token.literal.value.string)
  );

  *(scanner->current_token.tokenp) = '\0';
  *literalp = '\0';

  scanner->current_token.token = T_STRING;
  scanner->current_token.literal.type = STRING_LIT;
  scanner->current_token.literal.size = strlen(scanner->current_token.literal.value.string);
}

static void get_number(Scanner* scanner) {
  log_trace("Scanner::get_number");

  // assume larger unsigned 8 bytes
  long int whole_count = 0;
  long int decimal_offset = 0;
  long int exponent = 0;
  double nvalue = 0.0;
  double evalue = 0.0;

  scanner->digit_count = 0;
  scanner->current_token.token = NO_TOKEN;
  scanner->current_token.literal.type = I_64LIT;

  accumulate_value(scanner, &nvalue);

  log_trace("%f", nvalue);

  if (scanner->errored == TRUE) {
    return;
  }

  whole_count = scanner->digit_count;

  if (scanner->current_char == '.') {
    get_character(scanner);

    scanner->current_token.literal.type = F_64LIT;
    *(scanner->current_token.tokenp)++ = '.';

    accumulate_value(scanner, &nvalue);

    if (scanner->errored == TRUE) {
      return;
    }

    decimal_offset = whole_count - scanner->digit_count;
  }

  exponent = evalue + decimal_offset;

  if (exponent != 0) {
    nvalue *= pow(10, exponent);
  }

  log_info("Scanner::get_number exp(%ld)", exponent);
  log_info("Scanner::get_number FLT_DIG(%d)", FLT_DIG);

  if (scanner->current_token.literal.type == I_64LIT) {
    if (nvalue >= I_32_LOWER && nvalue <= I_32_UPPER) {
      log_info("Scanner::get_number i32");
      scanner->current_token.literal.type = I_32LIT;
      scanner->current_token.literal.value.i32 = nvalue;
      scanner->current_token.literal.size = sizeof(int);
    } else {
      log_info("Scanner::get_number i64");
      scanner->current_token.literal.value.i64 = nvalue;
      scanner->current_token.literal.size = sizeof(long int);
    }
  } else {
    if (labs(exponent) <= FLT_DIG && nvalue >= FLT_MIN && nvalue <= FLT_MAX) {
      log_info("Scanner::get_number f32");
      scanner->current_token.literal.type = F_32LIT;
      scanner->current_token.literal.value.f32 = nvalue;
      scanner->current_token.literal.size = sizeof(float);
    } else {
      log_info("Scanner::get_number f64");
      scanner->current_token.literal.value.f64 = nvalue;
      scanner->current_token.literal.size = sizeof(double);
    }
  }

  *(scanner->current_token.tokenp) = '\0';
  scanner->current_token.token = T_NUMERIC;
}

static void accumulate_value(Scanner *scanner, double *valuep) {
  log_trace("Scanner::accumulate_value");

  double value = *valuep;

  if (CHAR_CODE(scanner) != DIGIT) {
    scanner->current_token.token = T_ERROR;
    scanner->errored = TRUE;
    scanner->error_code = SCANNER_INVALID_NUMERIC_DEFINITION;
    return;
  }

  do {
    *(scanner->current_token.tokenp)++ = scanner->current_char;

    if (++scanner->digit_count <= DECIMAL_DIG) {
      value = 10 * value + (scanner->current_char - '0');
    } else {
      log_error("Scanner::accumulate_value diget overflow");
      scanner->errored = TRUE;
      scanner->error_code = SCANNER_PRECISION_LOST;
    }

    get_character(scanner);

  } while(CHAR_CODE(scanner) == DIGIT);

  *valuep = value;
}

static void get_special(Scanner *scanner) {
  log_trace("Scanner::get_special");

  *(scanner->current_token.tokenp) = scanner->current_char;

  switch(scanner->current_char) {
    case '(':   scanner->current_token.token = T_LPAREN; get_character(scanner);  break;
    case ')':   scanner->current_token.token = T_RPAREN; get_character(scanner);  break;
    case '=':   scanner->current_token.token = T_EQUAL;  get_character(scanner);  break;
    default:
      scanner->errored = TRUE;
      scanner->error_code = SCANNER_INVALID_TOKEN;
      scanner->current_token.token = T_ERROR;
  }

  *(scanner->current_token.tokenp) = '\0';
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
  .get_word = get_word,
  .get_string = get_string,
  .get_number = get_number,
  .get_special = get_special,
  .close = close
};