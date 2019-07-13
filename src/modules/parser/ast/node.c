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

void print_node_tree(ASTNodePtr root, int space) {
  if (root == NULL) { return; }

  space += PRINT_TREE_COUNT;

  print_node_tree(root->right, space);
  printf("\n");

  for (int i = PRINT_TREE_COUNT; i < space; i++) {
    printf(" ");
  }

  printf("(%d)\n", root->type);

  print_node_tree(root->left, space);
}

void free_node_tree(ASTNodePtr root) {
  if (root != NULL) {
    free_node_tree(root->left);
    free_node_tree(root->right);

    __FREE__(root->node);
    __FREE__(root);

    root->left = NULL;
    root->right = NULL;
    root = NULL;
  }
}