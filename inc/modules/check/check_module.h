#ifndef __GAMBIT_CHECK_MODULEH
#define __GAMBIT_CHECK_MODULEH 1

// ============================
//           Includes
// ============================

#include "common.h"
#include "util/file/file_module.h"

// ============================
//       Structs & Types
// ============================

struct check_module {
  void (*trace_error)(
    const char* error,
    const char* message,
    const char* filename,
    char **suggestions,
    unsigned int line_number,
    unsigned int buffer_offset);

};

// ============================
//          Externs
// ============================

extern const struct check_module CheckModule;

#endif