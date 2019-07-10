#include "modules/parser/ast/node.h"

ASTNodePtr build_node(NODE_TYPE type, void* node) {
  ASTNodePtr np = __MALLOC__(sizeof(ASTNode));
  np->type = type;
  np->node = node;

  return np;
}