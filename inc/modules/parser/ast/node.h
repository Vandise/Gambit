#ifndef __GAMBIT_AST_NODEH
#define __GAMBIT_AST_NODEH 1

#include "common.h"
#include "shared/tokens.h"

#define PRINT_TREE_COUNT 10

typedef enum {
  NOOP_NODE, LITERAL_NODE, GET_LOCAL_NODE, BINARY_OP_NODE, UNARY_OP_NODE, VARIABLE_DECLARATION_NODE, ASSIGNMENT_NODE
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
  struct ASTNodeStruct *left, *right;
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

//
// GetLocalNode, for getting local definitions
//
typedef struct GetLocalNodeStruct {
  char* identifier;
} GetLocalNode, *GetLocalNodePtr;

//
// Binary Op Node: +, -, *, / >, <, >=, <=
//  todo: modulus
//
typedef struct BinaryOpNodeStruct {
  TOKEN_CODE op;
} BinaryOpNode, *BinaryOpNodePtr;

//
// Unary Op Node
//  - <expresson>, + <expression>
//
typedef struct UnaryOpNodeStruct {
  TOKEN_CODE op;
} UnaryOpNode, *UnaryOpNodePtr;

//
// Assignment Node
//   left: identifier
//   right: <expression>
//
typedef struct AssignmentNodeStruct {
  // todo: type info
} AssignmentNode, *AssignmentNodePtr;

ASTNodePtr build_node(NODE_TYPE type, void* node);
void print_node_tree(ASTNodePtr root, int space);
void free_node_tree(ASTNodePtr root);

#endif