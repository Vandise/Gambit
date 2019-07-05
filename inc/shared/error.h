#ifndef __GAMBIT_ERROR_ERRORH
#define __GAMBIT_ERROR_ERRORH 1

typedef struct GambitScanner Scanner;

typedef enum {
  NO_ERROR_SUCCESS,
  SYNTAX_ERROR
} ERROR_CODE;

extern char *error_messages[];

void scanner_error(Scanner *scanner, ERROR_CODE code);

#endif
