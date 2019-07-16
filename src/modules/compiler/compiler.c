#include "modules/compiler/compiler_module.h"

static COMPILER_STATUS_CODE compile_NOOP_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  return OK;
}

static COMPILER_STATUS_CODE compile_CORE_LOAD_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  emit_core_load(compiler->out_file, CORE_LOAD_REQUIRE);
  return OK;
}

static COMPILER_STATUS_CODE compile_LITERAL_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  LiteralNodePtr l = (LiteralNodePtr)ref->node;

  switch(l->type) {
    case INTEGER_LIT:
      emit_integer_literal(compiler->out_file, l->value.integer);
      break;
    case REAL_LIT:
      emit_real_literal(compiler->out_file, l->value.real);
      break;
    case STRING_LIT:
      emit_string_literal(compiler->out_file, l->value.stringp);
      break;
    default:
      return UNDEFINED_NODE;
  }

  return OK;
}

CompilerPtr init_compiler(char *file_name, ASTNodePtr tree) {
  CompilerPtr compiler = __MALLOC__(sizeof(Compiler));
  compiler->out_file = fopen(file_name, "w");
  compiler->errored = FALSE;

  compiler->tree = tree;
  compiler->current_node = compiler->tree;

  //
  // nodes
  //
  compiler->compile[NOOP_NODE] = compile_NOOP_NODE;
  compiler->compile[CORE_LOAD_NODE] = compile_CORE_LOAD_NODE;
  compiler->compile[LITERAL_NODE] = compile_LITERAL_NODE;

  return compiler;
}

COMPILER_STATUS_CODE compile(CompilerPtr compiler) {
  COMPILER_STATUS_CODE status = OK;

  while(compiler->current_node != NULL && status == OK) {

    if (compiler->compile[compiler->current_node->type] != NULL) {
      status = (compiler->compile[compiler->current_node->type])(compiler, compiler->current_node);
      next_node(compiler);
    } else {
      status = UNDEFINED_NODE;
    }
  }

  return status;
}

void next_node(CompilerPtr compiler) {
  compiler->current_node = compiler->current_node->next;
}

void exit_compiler(CompilerPtr compiler) {
  if (compiler->out_file != NULL) {
    fclose(compiler->out_file);
  }

  __FREE__(compiler);
}