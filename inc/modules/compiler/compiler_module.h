#ifndef __GAMBIT_COMPILER_MODULEH
#define __GAMBIT_COMPILER_MODULEH 1

#include "common.h"
#include "modules/parser/ast/node.h"
#include "modules/compiler/emitter.h"
#include "modules/compiler/symbol_table.h"

typedef enum {
  UNDEFINED_NODE, OK, UNDEFINED_VARIABLE, INVALID_BINARY_OPERATION,
  INVALID_UNARY_OPERATION, UNDEFINED_VAR_DECL_VALUE_TYPE, REDECLARED_VARIABLE_BIND
} COMPILER_STATUS_CODE;

typedef struct GambitContext {

  char* name;
  SymbolTablePtr symbol_table;
  struct GambitContext *next, *previous;

} Context, *ContextPtr;

typedef struct GambitCompiler {
  ASTNodePtr tree;
  ASTNodePtr current_node;
  FILE *out_file;

  ContextPtr root_context;
  ContextPtr current_context;

  BOOLEAN errored;
  COMPILER_STATUS_CODE (*compile[0xFF])(struct GambitCompiler*, ASTNodePtr);
} Compiler, *CompilerPtr;

CompilerPtr init_compiler(char *file_name, ASTNodePtr tree);
COMPILER_STATUS_CODE compile(CompilerPtr compiler);
void next_node(CompilerPtr compiler);
void exit_compiler(CompilerPtr compiler);
void free_context_tree(ContextPtr ctx);
ContextPtr init_context(char* name);

#endif
