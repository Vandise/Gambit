#include "modules/parser/terminators.h"

static ASTNodePtr noOpNode() {
  ASTNodePtr n = __MALLOC__(sizeof(ASTNode));
  n->type = NOOP_NODE;

  n->left = NULL;
  n->right = NULL;
  n->next = NULL;

  n->node = __MALLOC__(sizeof(NOOPNode));

  return n;
}

ASTNodePtr terminator(Parser* parser) {
  ASTNodePtr node = NULL;

  switch( parser->current_token->code  ) {

    case T_NEWLINE: {
      node = noOpNode();
      next_token(parser);
      break;
    }

    default: {
      break;
    }

  }

  return node;
}