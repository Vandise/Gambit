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
  scanner->char_table['_'] = UNDERSCORE;
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

void get_token(Scanner* scanner) {
  skip_blanks(scanner);

  scanner->current_token.tokenp = scanner->current_token.token_string;

  switch(CHAR_CODE(scanner)) {
    case NEWLINE:
      scanner->current_token.token = T_NEWLINE;
      *(scanner->current_token.tokenp) = '\0';
      get_character(scanner);
      break;
    case LETTER:
      get_word(scanner, FALSE);
      break;
    case UPPERCASE_LETTER:
      get_word(scanner, TRUE);
      break;
    case DIGIT:
      get_number(scanner);
      break;
    case QUOTE:
        get_string(scanner);
        break;
    case EOF_CODE:
      scanner->current_token.token = T_END_OF_FILE;
      break;
    default: get_special(scanner);
  }
}

void get_string(Scanner* scanner) {
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

  *(scanner->current_token.tokenp) = '\0';
  *literalp = '\0';
  scanner->current_token.token = T_STRING;
  scanner->current_token.literal.type = STRING_LIT;
}

void get_word(Scanner* scanner, BOOLEAN is_constant) {
  while(CHAR_CODE(scanner) == LETTER || CHAR_CODE(scanner) == UPPERCASE_LETTER || CHAR_CODE(scanner) == UNDERSCORE) {
    *(scanner->current_token.tokenp)++ = scanner->current_char;
    get_character(scanner);
  }

  *(scanner->current_token.tokenp) = '\0';

  if(!string_is_reserved_word(scanner->current_token.token_string)) {
    if (is_constant) {
      scanner->current_token.token = T_CONSTANT;
    } else {
      scanner->current_token.token = T_IDENTIFIER;
    }
  } else {
    scanner->current_token.token = get_token_code(scanner->current_token.token_string);
  }
}

void get_special(Scanner *scanner) {
  *(scanner->current_token.tokenp) = scanner->current_char;

  switch(scanner->current_char) {
    case '(':   scanner->current_token.token = T_LPAREN; get_character(scanner);  break;
    case ')':   scanner->current_token.token = T_RPAREN; get_character(scanner);  break;
    case ',':   scanner->current_token.token = T_COMMA;  get_character(scanner);  break;
    case '=':   scanner->current_token.token = T_EQUAL;  get_character(scanner);  break;
    case '^':   scanner->current_token.token = T_PIN;    get_character(scanner);  break;
    case '*':   scanner->current_token.token = T_STAR;   get_character(scanner);  break;
    case '-':   scanner->current_token.token = T_MINUS;  get_character(scanner);  break;
    case '+':   scanner->current_token.token = T_PLUS;   get_character(scanner);  break;
    case '[':   scanner->current_token.token = T_LBRACKET;	get_character(scanner);  break;
    case ']':   scanner->current_token.token = T_RBRACKET;	get_character(scanner);  break;
    case ';':   scanner->current_token.token = T_SEMICOLON; get_character(scanner);  break;
    case '/':   scanner->current_token.token = T_SLASH;     get_character(scanner);  break;
    case '.':   scanner->current_token.token = T_PERIOD;     get_character(scanner);  break;
    case '<':
      get_character(scanner);
      if (scanner->current_char == '=') {
        *(scanner->current_token.tokenp)++ = '=';
        scanner->current_token.token= T_LE;
        get_character(scanner);
      } else {
        scanner->current_token.token = T_LT;
      }
      break;
    case '>':
      get_character(scanner);
      if (scanner->current_char == '=') {
        *(scanner->current_token.tokenp)++ = '=';
        scanner->current_token.token = T_GE;
        get_character(scanner);
      } else {
        scanner->current_token.token = T_GT;
      }
      break;
    default:
      scanner->current_token.token = T_ERROR;
  }

  *(scanner->current_token.tokenp) = '\0';
}

void get_number(Scanner* scanner) {
int whole_count = 0;
  int decimal_offset = 0;
  char exponent_sign = '+';
  int exponent = 0;
  float nvalue = 0.0;
  float evalue = 0.0;
  BOOLEAN saw_dotdot = FALSE;

  scanner->digit_count = 0;
  scanner->count_error = FALSE;
  scanner->current_token.token = NO_TOKEN;

  scanner->current_token.literal.type = INTEGER_LIT;

  // accumulate_value
  accumulate_value(scanner, &nvalue);

  if (scanner->current_token.token == T_ERROR) {
    return;
  }

  whole_count = scanner->digit_count;

  if (scanner->current_char == '.') {
    get_character(scanner);

    if (scanner->current_char == '.') {
      saw_dotdot = TRUE;
      --scanner->source_bufferp;
    } else {
      scanner->current_token.literal.type = REAL_LIT;
      *(scanner->current_token.tokenp)++ = '.';

      // accumulate_value
      accumulate_value(scanner, &nvalue);

      if (scanner->current_token.token == T_ERROR) {
        return;
      }

      decimal_offset = whole_count - scanner->digit_count;
    }
  }

  if (!saw_dotdot && ((scanner->current_char == 'E') || (scanner->current_char == 'e'))) {
    scanner->current_token.literal.type = REAL_LIT;
    *(scanner->current_token.tokenp)++ = scanner->current_char;
    get_character(scanner);

    if ((scanner->current_char == '+') || (scanner->current_char == '-')) {
      *(scanner->current_token.tokenp)++ = exponent_sign = scanner->current_char;
      get_character(scanner);
    }

    // accumulate_value
    accumulate_value(scanner, &evalue);

    if (scanner->current_token.token == T_ERROR) {
      return;
    }

    if (exponent_sign == '-') {
      evalue = -evalue;
    }
  }

  if(scanner->count_error) {
    scanner->current_token.token = T_ERROR;
    return;
  }

  exponent = evalue + decimal_offset;

  if ((exponent + whole_count < -MAX_EXPONENT) || (exponent + whole_count >  MAX_EXPONENT)) {
    scanner->current_token.token = T_ERROR;
    return;
  }

  if (exponent != 0) {
    nvalue *= pow(10, exponent);
  }

  if (scanner->current_token.literal.type == INTEGER_LIT) {
    if ((nvalue < -MAX_INTEGER) || (nvalue > MAX_INTEGER)) {
      scanner->current_token.token = T_ERROR;
      return;
    }
    scanner->current_token.literal.value.integer = nvalue;
  } else {
    scanner->current_token.literal.value.real = nvalue;
  }

  *(scanner->current_token.tokenp) = '\0';
  scanner->current_token.token = T_NUMBER;
}

/*
void get_number(Scanner* scanner) {
  int whole_count = 0;
  int decimal_offset = 0;
  char exponent_sign = '+';
  float nvalue = 0.0;

  scanner->digit_count = 0;
  scanner->count_error = FALSE;

  scanner->current_token.token = NO_TOKEN;
  scanner->current_token.literal.type = INTEGER_LIT;

  accumulate_value(scanner, &nvalue);

  if (scanner->current_token.token == T_ERROR) {
    return;
  }

  whole_count = scanner->digit_count;

  if (scanner->current_char == '.') {
    get_character(scanner);
    scanner->current_token.literal.type = REAL_LIT;
    *(scanner->current_token.tokenp)++ = '.';

    accumulate_value(scanner, &nvalue);

    if (scanner->current_token.token == T_ERROR) {
      return;
    }

    decimal_offset = whole_count - scanner->digit_count;
  }

  if (scanner->count_error) {
    scanner->current_token.token = T_ERROR;
    return;
  }

  if (scanner->current_token.literal.type == INTEGER_LIT) {
    scanner->current_token.literal.value.integer = nvalue;
  } else {
    scanner->current_token.literal.value.real = nvalue;
  }

  *(scanner->current_token.tokenp) = '\0';
  scanner->current_token.token = T_NUMBER;
}
*/


void accumulate_value(Scanner *scanner, float *valuep) {
  float value = *valuep;

  if (CHAR_CODE(scanner) != DIGIT) {
    scanner->current_token.token = T_ERROR;
    return;
  }

  do {
    *(scanner->current_token.tokenp)++ = scanner->current_char;

    if (++scanner->digit_count <= MAX_DIGIT_COUNT) {
      value = 10 * value + (scanner->current_char - '0');
    } else {
      scanner->count_error = TRUE;
    }

    get_character(scanner);

  } while(CHAR_CODE(scanner) == DIGIT);

  *valuep = value;
}

void commit_token(Scanner* scanner) {
  Token t = {
    .line_number = scanner->line_number,
    .level = scanner->level,
    .source_name = '\0',
    .word_string = '\0',
    .token_string = '\0',
    .code = scanner->current_token.token,
    .literal = scanner->current_token.literal
  };

  strcpy(t.source_name, scanner->source_name);
  strcpy(t.word_string, scanner->current_token.word_string);
  strcpy(t.token_string, scanner->current_token.token_string);

  insert_token_array(&(scanner->tokens), t);
}

TokenArray* get_tokens(Scanner* scanner) {
  return &(scanner->tokens);
}

void exit_scanner(Scanner *scanner) {
  if (scanner->source_file != NULL) {
    fclose(scanner->source_file);
  }
  free_token_array(&(scanner->tokens));
  __FREE__(scanner);
}
