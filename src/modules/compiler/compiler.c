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

//TOKEN_CODE add_op_list[] = { T_PLUS, T_MINUS, 0 };
//TOKEN_CODE mult_op_list[] = { T_STAR, T_SLASH, 0 };

static COMPILER_STATUS_CODE compile_BINARY_OP_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  BinaryOpNodePtr n = (BinaryOpNodePtr)ref->node;
  char op[] = "\0\0\0";

  switch(n->op) {
    case T_AND:
      op[0] = op[1] = '&';
      break;
    case T_OR:
      op[0] = op[1] = '|';
      break;
    case T_EQUAL:
      op[0] = op[1] = '=';
      break;
    case T_GT:
      op[0] = '>';
      break;
    case T_LT:
      op[0] = '<';
      break;
    case T_GE:
      op[0] = '>';
      op[1] = '=';
      break;
    case T_LE:
      op[0] = '<';
      op[1] = '=';
      break;
    case T_PLUS:
      op[0] = '+';
      break;
    case T_MINUS:
      op[0] = '-';
      break;
    case T_STAR:
      op[0] = '*';
      break;
    case T_SLASH:
      op[0] = '/';
      break;
    default:
      compiler->errored = TRUE;
      return INVALID_BINARY_OPERATION;
  }

  if (!compiler->errored) {
    emit_text(compiler->out_file, "(");
    if (ref->left != NULL) {
      (compiler->compile[ref->left->type])(compiler, ref->left);
    }

    emit_text(compiler->out_file, op);

    if (ref->right != NULL) {
      (compiler->compile[ref->right->type])(compiler, ref->right);
    }
    emit_text(compiler->out_file, ")");
  }

  return OK;
}

static COMPILER_STATUS_CODE compile_UNARY_OP_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  UnaryOpNodePtr n = (UnaryOpNodePtr)ref->node;

  char op[] = "\0\0";

  switch(n->op) {
    case T_PLUS:
      op[0] = '+';
      break;
    case T_MINUS:
      op[0] = '-';
      break;
    default:
      compiler->errored = TRUE;
      return INVALID_UNARY_OPERATION;
  }

  if (!compiler->errored) {
    emit_text(compiler->out_file, "(");
    if (ref->left != NULL) {
      (compiler->compile[ref->left->type])(compiler, ref->left);
    }

    emit_text(compiler->out_file, op);

    if (ref->right != NULL) {
      (compiler->compile[ref->right->type])(compiler, ref->right);
    }
    emit_text(compiler->out_file, ")");
  }

  return OK;
}

static COMPILER_STATUS_CODE compile_VARIABLE_DECLARATION_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  VariableDeclarationNodePtr n = (VariableDeclarationNodePtr)ref->node;

  LiteralNodePtr identifier_node = (LiteralNodePtr)ref->left->node;
  char* identifier = identifier_node->value.stringp;

  //
  // todo: verify identifier does not exist in the symbol table ( redefinition error )
  //

  switch(n->definition.info.variable.type) {

    case VARIABLE_SIMPLE_ASSIGN: {
      emit_var_declaration(compiler->out_file, identifier);

      if (ref->right != NULL) {
        emit_text(compiler->out_file, "=");
        (compiler->compile[ref->right->type])(compiler, ref->right);
      }

      emit_terminator(compiler->out_file);
      break;
    }

    case VARIABLE_MATCH_ALL: {
      emit_var_declaration(compiler->out_file, identifier);
      emit_text(compiler->out_file, "=");
      emit_var_declaration_match_all(compiler->out_file, identifier);

      emit_text(compiler->out_file, "(");
      (compiler->compile[ref->right->type])(compiler, ref->right);
      emit_text(compiler->out_file, ")");

      emit_terminator(compiler->out_file);
      break;
    }

    case VARIABLE_MATCH_VALUE: {
      emit_var_declaration(compiler->out_file, identifier);
      emit_text(compiler->out_file, "=");

      switch(n->definition.info.variable.value_type) {
        case STRING_LIT: {
          char* str_value = n->definition.info.variable.value.stringp;
          emit_var_declaration_match_string(compiler->out_file, str_value, identifier);
          emit_text(compiler->out_file, "(");
          (compiler->compile[ref->right->type])(compiler, ref->right);
          emit_text(compiler->out_file, ")");

          emit_terminator(compiler->out_file);
          break;
        }

        case INTEGER_LIT: {
          int value = n->definition.info.variable.value.integer;
          emit_var_declaration_match_int(compiler->out_file, value, identifier);
          emit_text(compiler->out_file, "(");
          (compiler->compile[ref->right->type])(compiler, ref->right);
          emit_text(compiler->out_file, ")");

          emit_terminator(compiler->out_file);
          break;
        }

        case REAL_LIT: {
          float value = n->definition.info.variable.value.real;
          emit_var_declaration_match_float(compiler->out_file, value, identifier);
          emit_text(compiler->out_file, "(");
          (compiler->compile[ref->right->type])(compiler, ref->right);
          emit_text(compiler->out_file, ")");

          emit_terminator(compiler->out_file);
          break;
        }

        default:
          compiler->errored = TRUE;
          return UNDEFINED_VAR_DECL_VALUE_TYPE;
          break;
      }

      break;
    }

    case VARIABLE_MATCH_CONSTANT:
      emit_var_declaration(compiler->out_file, identifier);
      emit_text(compiler->out_file, "=");

      char* constant = n->definition.info.variable.value.stringp;
      emit_var_declaration_match_const(compiler->out_file, constant, identifier);

      emit_text(compiler->out_file, "(");
      (compiler->compile[ref->right->type])(compiler, ref->right);
      emit_text(compiler->out_file, ")");

      emit_terminator(compiler->out_file);
      break;
    default:
      compiler->errored = TRUE;
      return INVALID_UNARY_OPERATION;
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
  compiler->compile[UNARY_OP_NODE] = compile_UNARY_OP_NODE;
  compiler->compile[VARIABLE_DECLARATION_NODE] = compile_VARIABLE_DECLARATION_NODE;

  return compiler;
}

COMPILER_STATUS_CODE compile(CompilerPtr compiler) {
  COMPILER_STATUS_CODE status = OK;

  while(compiler->errored == FALSE && compiler->current_node != NULL && status == OK) {

    if (compiler->compile[compiler->current_node->type] != NULL) {
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