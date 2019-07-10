/*
#include "modules/parser/ast/symbol_table.h"

// JS Primitives

TypeStructPtr IntegerTypePtr, RealTypePtr, BooleanTypePtr, CharTypePtr;

ASTNodePtr search_symbol_table(const char* name, ASTNodePtr nodep) {
  int compare;

  while(nodep != NULL) {
    compare = strcmp(name, nodep->name);
    if (compare == 0) { return nodep; }
    nodep = compare < 0 ? nodep->left : nodep->right;
  }
  return NULL;
}

ASTNodePtr insert_symbol_table(const char* name, ASTNodePtr *nodepp) {
  int compare;
  ASTNodePtr new_nodep;
  ASTNodePtr nodep;

  new_nodep = __MALLOC__(sizeof(ASTNode));
  new_nodep->name = __MALLOC__(sizeof(name) + 1);
  strcpy(new_nodep->name, name);

  new_nodep->left = new_nodep->right = new_nodep->next = NULL;
  new_nodep->info = NULL;
  new_nodep->defn.key = UNDEFINED;
  new_nodep->level = new_nodep->label_index = 0;

  while( (nodep = *nodepp) != NULL) {
    compare = strcmp(name, nodep->name);
    nodepp = compare < 0 ? &(nodep->left) : &(nodep->right);
  }

  *nodepp = new_nodep;

  return new_nodep;
}

void free_symbol_table_node(ASTNodePtr nodep) {
  __FREE__(nodep->name);
  __FREE__(nodep);
  nodep = NULL;
}

void free_symbol_type(TypeStructPtr type) {
  __FREE__(type);
}

void free_ast(ASTNodePtr node) {
  if (node != NULL) {
    free_ast(node->right);
    __FREE__(node->name);
    free_ast(node->left);
    __FREE__(node);
    node = NULL;
  }
}

void exit_symbol_table(ASTNodePtr root_node) {
  free_ast(root_node);
  free_symbol_type(IntegerTypePtr);
  free_symbol_type(RealTypePtr);
  free_symbol_type(BooleanTypePtr);
  free_symbol_type(CharTypePtr);
}

void init_symbol_table(ASTNodePtr *root_node) {
  ASTNodePtr integer_idp, real_idp, boolean_idp, char_idp,
      false_idp, true_idp;

  enter_name_local_symtab(integer_idp, "Integer", root_node);
  enter_name_local_symtab(real_idp,    "Real", root_node);
  enter_name_local_symtab(boolean_idp, "Boolean", root_node);
  enter_name_local_symtab(char_idp,    "Char", root_node);
  enter_name_local_symtab(false_idp,   "False", root_node);
  enter_name_local_symtab(true_idp,    "True", root_node);

  IntegerTypePtr = __MALLOC__(sizeof(TypeStruct));
  RealTypePtr    = __MALLOC__(sizeof(TypeStruct));
  BooleanTypePtr = __MALLOC__(sizeof(TypeStruct));
  CharTypePtr    = __MALLOC__(sizeof(TypeStruct));

  integer_idp->defn.key   = TYPE_DEFN;
  integer_idp->typep      = IntegerTypePtr;
  IntegerTypePtr->form     = SCALAR_FORM;
  IntegerTypePtr->size     = sizeof(int);
  IntegerTypePtr->type_idp = integer_idp;

  real_idp->defn.key      = TYPE_DEFN;
  real_idp->typep         = RealTypePtr;
  RealTypePtr->form        = SCALAR_FORM;
  RealTypePtr->size        = sizeof(float);
  RealTypePtr->type_idp    = real_idp;

  boolean_idp->defn.key   = TYPE_DEFN;
  boolean_idp->typep      = BooleanTypePtr;
  BooleanTypePtr->form     = ENUM_FORM;
  BooleanTypePtr->size     = sizeof(int);
  BooleanTypePtr->type_idp = boolean_idp;

  BooleanTypePtr->info.enumeration.max = 1;
  boolean_idp->typep->info.enumeration.const_idp = false_idp;
  false_idp->defn.key = CONST_DEFN;
  false_idp->defn.info.constant.value.integer = 0;
  false_idp->typep = BooleanTypePtr;

  false_idp->next = true_idp;
  true_idp->defn.key = CONST_DEFN;
  true_idp->defn.info.constant.value.integer = 1;
  true_idp->typep = BooleanTypePtr;

  char_idp->defn.key   = TYPE_DEFN;
  char_idp->typep      = CharTypePtr;
  CharTypePtr->form     = SCALAR_FORM;
  CharTypePtr->size     = sizeof(char);
  CharTypePtr->type_idp = char_idp;
}
*/
