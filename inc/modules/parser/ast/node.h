#ifndef __GAMBIT_AST_NODEH
#define __GAMBIT_AST_NODEH 1

#include "common.h"

//https://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers

typedef enum {
  UNDEFINED_NODE, VARIABLE_DECLARATION_NODE
} NODE_TYPE;

typedef union {
  int integer;
  float real;
  char character;
  char* stringp;
} Value;

//
// High-Level node, every node is an ASTNode
//
typedef ASTNodeStruct {
  NODE_TYPE type;
  void* node;
} ASTNode;

//
// 
//
typedef VariableDeclarationNodeStruct {
  
} VariableDeclarationNode, *VariableDeclarationNodePtr;

#endif