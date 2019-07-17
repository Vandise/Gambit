#include "modules/compiler/symbol_table.h"

SymbolTablePtr init_symbol_table() {
  SymbolTablePtr table = __MALLOC__(sizeof(SymbolTable));
  table->locals = NULL;
  table->globals = NULL;

  // todo: track constant value in info
  insert_symbol_table("GAMIT_VERSION", &table->globals);

  return table;
}

SymbolTableNodePtr search_symbol_table(const char *name, SymbolTableNodePtr nodep) {
  int compare;

  while(nodep != NULL) {
    compare = strcmp(name, nodep->name);
    if (compare == 0) {
      return nodep;
    }
    nodep = compare < 0 ? nodep->left : nodep->right;
  }

  return NULL;
}

SymbolTableNodePtr insert_symbol_table(const char *name, SymbolTableNodePtr *nodepp) {
  int compare;
  SymbolTableNodePtr new_nodep;
  SymbolTableNodePtr nodep;

  new_nodep = __MALLOC__(sizeof(SymbolTableNode));
  new_nodep->name = __MALLOC__(strlen(name) + 1);
  strcpy(new_nodep->name, name);

  new_nodep->left = new_nodep->right = NULL;

  while ((nodep = *nodepp) != NULL) {
    compare = strcmp(name, nodep->name);
    nodepp = compare < 0 ? &(nodep->left) : &(nodep->right);
  }

  *nodepp = new_nodep;

  return new_nodep;
}

static void free_symbol_table_ast(SymbolTableNodePtr node) {
  if (node != NULL) {
    free_symbol_table_ast(node->right);

    __FREE__(node->name);

    free_symbol_table_ast(node->left);
    __FREE__(node);

    node = NULL;
  }
}

void free_symbol_table(SymbolTablePtr table) {
  free_symbol_table_ast(table->locals);
  free_symbol_table_ast(table->globals);

  __FREE__(table);
}