#ifndef __GAMBIT_LIB__
#define __GAMBIT_LIB__ 1

#define __GAMBITLIB_LIST_SIZE 1
#define __GAMBITLIB_LOAD_FN "load_module"

#include <dlfcn.h>
#include "common.h"
#include "modules/error.h"

typedef struct gambit_lib gambit_lib;
typedef struct gambit_lib_module gambit_lib_module;

typedef void* load_lib_defn();

struct gambit_lib {
  uint8_t version;
  const char* name;
  void* instance;
};

struct gambit_lib_module {
  uint8_t lib_list_size;
  BOOLEAN initialized;
  gambit_lib libraries[__GAMBITLIB_LIST_SIZE];

  void (*initialize)(gambit_lib_module*);
  error_module* (*get_error_module)(gambit_lib_module*);
};

extern gambit_lib_module GambitLibModule;

#endif