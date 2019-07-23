#include "modules/compiler/emitter.h"

void emit_text(FILE *fptr, char* text) {
  fprintf(fptr, "%s", text);
}

void emit_terminator(FILE *fptr) {
  emit_text(fptr, ";");
}

void emit_core_load(FILE *fptr, char* core_require) {
  char* coreload = "const match = %s;\nconst $ = match.parameter;\nconst _ = match.wildcard;\n";
  fprintf(fptr, coreload, core_require);
}

void emit_integer_literal(FILE *fptr, int value) {
  char* int_lit = "%d";
  fprintf(fptr, int_lit, value);
}

void emit_real_literal(FILE *fptr, float value) {
  char* real_lit = "%f";
  fprintf(fptr, real_lit, value);
}

void emit_string_literal(FILE *fptr, char* value) {
  char* string_lit = "\"%s\"";
  fprintf(fptr, string_lit, value);
}

void emit_var_declaration(FILE *fptr, char* identifier) {
  fprintf(fptr, "const %s", identifier);
}

void emit_var_declaration_match_all(FILE *fptr, char* identifier) {
  fprintf(fptr, "match([$,(%s)=>%s])", identifier, identifier);
}

void emit_var_declaration_match_const(FILE *fptr, char* constant, char* identifier) {
  fprintf(fptr, "match([%s,(%s)=>%s])", constant, identifier, identifier);
}

void emit_var_declaration_match_string(FILE *fptr, char* value, char* identifier) {
  fprintf(fptr, "match([$(\"%s\"),(%s)=>%s])", value, identifier, identifier);
}

void emit_var_declaration_match_int(FILE *fptr, int value, char* identifier) {
  fprintf(fptr, "match([$(%d),(%s)=>%s])", value, identifier, identifier);
}

void emit_var_declaration_match_float(FILE *fptr, float value, char* identifier) {
  fprintf(fptr, "match([$(%f),(%s)=>%s])", value, identifier, identifier);
}
