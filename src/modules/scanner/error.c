#include "modules/scanner/error.h"

const char* scanner_error_message[] = {
  "Unknown Error",
  "Scanner failed to open file",
  "Scanner failed to parse source line",
  "Scanner failed to parse numeric definition",
  "Scanner failed to preserve numeric precision",
  "Scanner failed to scan token"
};

const char* scanner_error_suggestions[][3] = {
  { "Please submit an issue on Github providing example code.", NULL, NULL },
  { "Ensure the file you're trying to run exists.", NULL, NULL },
  { "Limit the number of characters per line to 1024.", NULL, NULL },
  {
    "Ensure the defined numeric is a 32 or 64 bit value.",
    "Utilize the Math module for exponents and mathematical operations.",
    NULL
  },
  { "Limit Floating-point precision.", NULL, NULL },
  { "Review syntax.", "Ensure that you're using only UTF-8 characters.", NULL }
};