#ifndef __GAMBIT_COMPILER_MODULE_EMITTERH
#define __GAMBIT_COMPILER_MODULE_EMITTERH 1

#include "common.h"

#define CORE_LOAD_REQUIRE "require('./core/match')"

void emit_core_load(FILE *fptr, char* core_require);

#endif
