#include "modules/parser/ast/node.h"

ASTNodePtr build_node(NODE_TYPE type, void* node) {
  ASTNodePtr np = __MALLOC__(sizeof(ASTNode));
  np->type = type;
  np->node = node;
  np->left = NULL;
  np->right = NULL;
  np->next = NULL;

  return np;
}

void free_node_tree(ASTNodePtr root) {
  if (root != NULL) {
    free_node_tree(root->left);
    free_node_tree(root->right);

    __FREE__(root->node);
    __FREE__(root);

    root = NULL;
  }
}