#ifndef __GAMBIT_AST_NODEH
#define __GAMBIT_AST_NODEH 1

#include "common.h"
#include "shared/tokens.h"

typedef enum {
  NOOP_NODE, LITERAL_NODE, VARIABLE_DECLARATION_NODE
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
typedef struct ASTNodeStruct {
  NODE_TYPE type;
  void* node;
  struct ASTNodeStruct *next;
} ASTNode, *ASTNodePtr;

//
// NO-OP Node - used for root and debugging
//
typedef struct NOOPNodeStruct {

} NOOPNode, *NOOPNodePtr;

//
// Literal node for int, real, character, and string
//
typedef struct LiteralNodeStruct {
  LITERAL_TYPE type;
  Value value;
} LiteralNode, *LiteralNodePtr;

#endif