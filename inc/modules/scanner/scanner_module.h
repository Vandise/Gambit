#ifndef __GAMBIT_SCANNER_MODULEH
#define __GAMBIT_SCANNER_MODULEH 1

// ============================
//           Includes
// ============================

#include "common.h"
#include "modules/scanner/shared.h"
#include "modules/scanner/error.h"

// ============================
//           Macros
// ============================

#define CHAR_CODE(scanner) scanner->char_table[scanner->current_char]
#define GAMBIT_SCANNER_VERSION 100

// ============================
//          Externs
// ============================

extern const struct scanner_module ScannerModule;

// ============================
//       Structs & Types
// ============================

typedef struct GambitScanner {
  unsigned int line_number;
  unsigned int level;
  unsigned int buffer_offset;

  CHAR_CODE char_table[256];

  char source_buffer[1024];
  char source_name[64];
  FILE *source_file;

  char *source_bufferp;
  char current_char;

  BOOLEAN errored;
  SCANNER_ERROR_CODE error_code;
} Scanner;

struct scanner_module {
  const signed int version;

  Scanner* (*init)(const char*);

  BOOLEAN (*open_source_file)(Scanner*);
  BOOLEAN (*get_source_line)(Scanner*);

  void (*get_character)(Scanner*);
  void (*skip_comment)(Scanner*);
  void (*skip_blanks)(Scanner*);

  void (*close)(Scanner*);
};

// ============================
//      Forwards/Globals
// ============================


#endif