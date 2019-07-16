#include "modules/compiler/emitter.h"

void emit_core_load(FILE *fptr, char* core_require) {
  char* coreload = "const match = %s;\nconst $ = match.parameter;\nconst _ = match.wildcard;\n";
  fprintf(fptr, coreload, core_require);
}