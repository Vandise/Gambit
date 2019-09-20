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
      emit_string_literal(compiler->out_file, l->value.stringp, l->asParam);
      break;
    default:
      compiler->errored = TRUE;
      return UNDEFINED_NODE;
  }

  return OK;
}

static COMPILER_STATUS_CODE compile_GET_LOCAL_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  GetLocalNodePtr n = (GetLocalNodePtr)ref->node;

  if (search_symbol_table(n->identifier, compiler->current_context->symbol_table->locals) == NULL) {
    compiler->errored = TRUE;
    return UNDEFINED_VARIABLE;
  }

  emit_string_literal(compiler->out_file, n->identifier, TRUE);

  return OK;
}

// match([c, (d) => true])(b);
static COMPILER_STATUS_CODE compile_BINARY_OP_NODE_MATCH(CompilerPtr compiler, ASTNodePtr ref) {

  emit_text(compiler->out_file, "match([");
  (compiler->compile[ref->left->type])(compiler, ref->left);
  emit_text(compiler->out_file, ",()=>true])(");
  (compiler->compile[ref->right->type])(compiler, ref->right);
  emit_text(compiler->out_file, ")");
  emit_terminator(compiler->out_file);

  return OK;
}

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
      return compile_BINARY_OP_NODE_MATCH(compiler, ref);
      //op[0] = op[1] = '=';
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

  if ( search_symbol_table(identifier, compiler->current_context->symbol_table->locals) != NULL ) {
    compiler->errored = TRUE;
    return REDECLARED_VARIABLE_BIND;
  } else {
    insert_symbol_table(identifier, &(compiler->current_context->symbol_table->locals));
  }

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

static COMPILER_STATUS_CODE compile_STRUCT_DECLARATION_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  StructDeclarationNodePtr n = (StructDeclarationNodePtr)ref->node;
  ASTNodePtr currentNode = NULL;

  LiteralNodePtr identifier_node = (LiteralNodePtr)ref->left->node;
  char* identifier = identifier_node->value.stringp;

  if ( search_symbol_table(identifier, compiler->current_context->symbol_table->constants) != NULL ) {
    compiler->errored = TRUE;
    return REDECLARED_CONSTANT;
  } else {
    insert_symbol_table(identifier, &(compiler->current_context->symbol_table->constants));
  }

  emit_struct_prefix_declaration(compiler->out_file, identifier);
  emit_text(compiler->out_file, "(");

  currentNode = ref->right;
  while(currentNode != NULL) {
    (compiler->compile[currentNode->left->type])(compiler, currentNode->left);
    if (currentNode->right != NULL) {
      emit_text(compiler->out_file, ",");
    }
    currentNode = currentNode->right;
  }

  emit_text(compiler->out_file, "){");

  currentNode = ref->right;
  while(currentNode != NULL) {
    emit_struct_this_dot(compiler->out_file);
    (compiler->compile[currentNode->left->type])(compiler, currentNode->left);

    emit_text(compiler->out_file, "=");

    (compiler->compile[currentNode->left->type])(compiler, currentNode->left);

    emit_text(compiler->out_file, "||");

    StructPropertyNodePtr n = currentNode->node;
    (compiler->compile[n->body->type])(compiler, n->body);

    emit_text(compiler->out_file, ";");

    currentNode = currentNode->right;
  }

  emit_text(compiler->out_file, "};");

  return OK;
}

static COMPILER_STATUS_CODE compile_STRUCT_PROPERTY_NODE(CompilerPtr compiler, ASTNodePtr ref) {
  return OK;
}

CompilerPtr init_compiler(char *file_name, ASTNodePtr tree) {
  CompilerPtr compiler = __MALLOC__(sizeof(Compiler));
  compiler->out_file = fopen(file_name, "w");
  compiler->errored = FALSE;

  compiler->tree = tree;
  compiler->current_node = compiler->tree;

  compiler->root_context = init_context(MAIN_CONTEXT);
  compiler->current_context = compiler->root_context;

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
  compiler->compile[STRUCT_DECLARATION_NODE] = compile_STRUCT_DECLARATION_NODE;
  compiler->compile[STRUCT_PROPERTY_NODE] = compile_STRUCT_PROPERTY_NODE;

  return compiler;
}

ContextPtr init_context(char* name) {
  ContextPtr ctx = __MALLOC__(sizeof(Context));
  ctx->name = name;
  ctx->symbol_table = init_symbol_table();
  ctx->next = NULL;
  ctx->previous = NULL;

  return ctx;
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

void free_context_tree(ContextPtr root) {
  if (root != NULL) {
    free_context_tree(root->next);
    free_symbol_table(root->symbol_table);
    __FREE__(root);
    root = NULL;
  }
}

void exit_compiler(CompilerPtr compiler) {
  if (compiler->out_file != NULL) {
    fclose(compiler->out_file);
  }
  free_context_tree(compiler->root_context);
  __FREE__(compiler);
}