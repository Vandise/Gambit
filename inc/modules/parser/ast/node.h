#ifndef __GAMBIT_AST_NODEH
#define __GAMBIT_AST_NODEH 1

#include "common.h"
#include "shared/tokens.h"

#define PRINT_TREE_COUNT 10

typedef enum {
  NOOP_NODE, CORE_LOAD_NODE, LITERAL_NODE, GET_LOCAL_NODE, BINARY_OP_NODE, UNARY_OP_NODE, VARIABLE_DECLARATION_NODE, ASSIGNMENT_NODE,
  STRUCT_DECLARATION_NODE, STRUCT_PROPERTY_NODE
} NODE_TYPE;

typedef enum {
  DEFINITION_NO_TYPE,
  DEFINITION_VARIABLE,
} DEFINITION_TYPE;

//
// let(year)  = 2015          -> VARIABLE_SIMPLE_ASSIGN
// let($year) = 2015          -> VARIABLE_MATCH_BIND_ALL
// let($year:Number) = 2015   -> VARIABLE_MATCH_CONSTANT
// let($year:2015) = 2015     -> VARIABLE_MATCH_VALUE
//
typedef enum {
  VARIABLE_SIMPLE_ASSIGN, VARIABLE_MATCH_ALL, VARIABLE_MATCH_VALUE, VARIABLE_MATCH_CONSTANT
} VARIABLE_DECLARATION_TYPE;

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
// Definition Struct: variable type info
//
typedef struct {
  DEFINITION_TYPE type;
  union {
    struct {
      VARIABLE_DECLARATION_TYPE type;
      LITERAL_TYPE value_type;
      Value value;
    } variable;
  } info;
} NodeDefinition;

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
  BOOLEAN asParam;
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
  //
  // simple assign shortcut?
  // make everything immutable?
  //
} AssignmentNode, *AssignmentNodePtr;

//
// Variable Declaration Node
//  left: identifier
//  right: expression
//
//  body:
//    definition:  variable definition info
//    next: if variables declared in a chain -- let(a, b, c) = <expression>
//
typedef struct VariableDeclarationNodeStruct {
  NodeDefinition definition;
} VariableDeclarationNode, *VariableDeclarationNodePtr;

//
// Struct declaration
//
// left: identifier
// right: Properties tree
typedef struct StructDeclarationNodeStruct {

} StructDeclarationNode, *StructDeclarationNodePtr;

// left: identifier
// defaultValue: <factor>
// right: Properties tree
typedef struct StructPropertyNodeStruct {
  ASTNodePtr body;
} StructPropertyNode, *StructPropertyNodePtr;

ASTNodePtr build_node(NODE_TYPE type, void* node);
void print_node_tree(ASTNodePtr root, int space);
void free_node_tree(ASTNodePtr root);

#endif