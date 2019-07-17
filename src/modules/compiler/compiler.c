#include "modules/compiler/compiler_module.h"

static void compile_node_tree(CompilerPtr compiler, ASTNodePtr ref) {
  if (ref != NULL && compiler->errored == FALSE) {
    printf("\n Compile node tree \n");

    // left nodes
    compile_node_tree(compiler, ref->left);

    // current node
    (compiler->compile[ref->type])(compiler, ref);

    // right nodes
    compile_node_tree(compiler, ref->right);
  }
}

static COMPILER_STATUS_CODE compile_NOOP_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  return OK;
}

static COMPILER_STATUS_CODE compile_CORE_LOAD_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  printf("\n Compile core load node \n");
  emit_core_load(compiler->out_file, CORE_LOAD_REQUIRE);
  return OK;
}

static COMPILER_STATUS_CODE compile_LITERAL_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  printf("\n Compile literal node \n");
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
      compiler->errored = TRUE;
      return UNDEFINED_NODE;
  }

  return OK;
}

static COMPILER_STATUS_CODE compile_GET_LOCAL_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  // TODO:
  //    statements treat get_locals as assignment
  //
  //    T_EQUAL needs to be T_EQUAL_EQUAL? maybe small_arrow. 
  //
  // GetLocalNodePtr n = (GetLocalNodePtr)ref->node;
  //
  //
  return OK;
}

static COMPILER_STATUS_CODE compile_BINARY_OP_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  printf("\n Compile binary op node \n");
  BinaryOpNodePtr n = (BinaryOpNodePtr)ref->node;

  switch(n->op) {
    case T_EQUAL:
      compile_node_tree(compiler, ref->left);
      emit_text(compiler->out_file, "==");
      compile_node_tree(compiler, ref->right);
      break;
    case T_GT:
      compile_node_tree(compiler, ref->left);
      emit_text(compiler->out_file, ">");
      compile_node_tree(compiler, ref->right);
      break;
    case T_LT:
      compile_node_tree(compiler, ref->left);
      emit_text(compiler->out_file, "<");
      compile_node_tree(compiler, ref->right);
      break;
    case T_GE:
      compile_node_tree(compiler, ref->left);
      emit_text(compiler->out_file, ">=");
      compile_node_tree(compiler, ref->right);
      break;
    case T_LE:
      compile_node_tree(compiler, ref->left);
      emit_text(compiler->out_file, "<=");
      compile_node_tree(compiler, ref->right);
      break;
    default:
      compiler->errored = TRUE;
      return INVALID_BINARY_OPERATION;
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
  compiler->compile[GET_LOCAL_NODE] = compile_GET_LOCAL_NODE;
  compiler->compile[BINARY_OP_NODE] = compile_BINARY_OP_NODE;

  return compiler;
}

COMPILER_STATUS_CODE compile(CompilerPtr compiler) {
  COMPILER_STATUS_CODE status = OK;

  while(compiler->errored == FALSE && compiler->current_node != NULL && status == OK) {

    if (compiler->compile[compiler->current_node->type] != NULL) {
      printf("\n Compile current node: %d \n", compiler->current_node->type);
      status = (compiler->compile[compiler->current_node->type])(compiler, compiler->current_node);
      next_node(compiler);
    } else {
      status = UNDEFINED_NODE;
      compiler->errored = TRUE;
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