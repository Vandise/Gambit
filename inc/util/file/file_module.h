#ifndef __GAMBIT_SHARED_FILE_MODULEH
#define __GAMBIT_SHARED_FILE_MODULEH 1

// ============================
//           Includes
// ============================

#include "common.h"

// ============================
//       Structs & Types
// ============================

struct file_module {
  BOOLEAN (*get_file_real_path)(const char*, char*);
  BOOLEAN (*file_exists)(const char*);
};

// ============================
//          Externs
// ============================

extern const struct file_module FileModule;

#endif