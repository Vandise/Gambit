#ifndef __GAMBIT_SCANNER_ERRORH
#define __GAMBIT_SCANNER_ERRORH 1

// ============================
//           Includes
// ============================

#include "common.h"

// ============================
//       Structs & Types
// ============================

typedef enum {
    SCANNER_UNKNOWN,
    SCANNER_FAILED_TO_OPEN_FILE,
    SCANNER_FAILED_TO_LOAD_SRC_LINE,
    SCANNER_INVALID_NUMERIC_DEFINITION,
    SCANNER_PRECISION_LOST,
    SCANNER_INVALID_TOKEN
} SCANNER_ERROR_CODE;

// ============================
//          Externs
// ============================

extern const char* scanner_error_message[];
extern const char* scanner_error_suggestions[][3];

#endif