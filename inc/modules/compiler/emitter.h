#ifndef __GAMBIT_COMPILER_MODULE_EMITTERH
#define __GAMBIT_COMPILER_MODULE_EMITTERH 1

#include "common.h"

#define CORE_LOAD_REQUIRE "require('./core/match')"

void emit_text(FILE *fptr, char* text);
void emit_terminator(FILE *fptr);

void emit_core_load(FILE *fptr, char* core_require);
void emit_integer_literal(FILE *fptr, int value);
void emit_real_literal(FILE *fptr, float value);
void emit_string_literal(FILE *fptr, char* string);

void emit_var_declaration(FILE *fptr, char* identifier);
void emit_var_declaration_match_all(FILE *fptr, char* identifier);
void emit_var_declaration_match_const(FILE *fptr, char* constant, char* identifier);
void emit_var_declaration_match_string(FILE *fptr, char* value, char* identifier);
void emit_var_declaration_match_int(FILE *fptr, int value, char* identifier);
void emit_var_declaration_match_float(FILE *fptr, float value, char* identifier);

#endif
