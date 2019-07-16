#ifndef __GAMBIT_COMPILER_MODULEH
#define __GAMBIT_COMPILER_MODULEH 1

#include "common.h"
#include "modules/parser/ast/node.h"
#include "modules/compiler/emitter.h"

typedef enum {
  UNDEFINED_NODE, OK
} COMPILER_STATUS_CODE;

//
// TODO: context / symbol table
//
typedef struct GambitCompiler {
  ASTNodePtr tree;
  ASTNodePtr current_node;
  FILE *out_file;
  BOOLEAN errored;
  COMPILER_STATUS_CODE (*compile[0xFF])(struct GambitCompiler*, ASTNodePtr);
} Compiler, *CompilerPtr;

CompilerPtr init_compiler(char *file_name, ASTNodePtr tree);
COMPILER_STATUS_CODE compile(CompilerPtr compiler);
void next_node(CompilerPtr compiler);
void exit_compiler(CompilerPtr compiler);

#endif
