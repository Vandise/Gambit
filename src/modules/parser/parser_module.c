#include "modules/parser/parser_module.h"

Parser* init_parser(TokenArray* tokens) {
  Parser* parser = __MALLOC__(sizeof(Parser));
  parser->token_array = tokens;
  parser->errored = FALSE;

  parser->root_node = __MALLOC__(sizeof(ASTNode));
  parser->root_node->type = NOOP_NODE;
  parser->root_node->node = __MALLOC__(sizeof(NOOPNode));
  parser->root_node->next = NULL;
  parser->current_node = parser->root_node;

  parser->current_token = parser->token_array->array;

  return parser;
}

Token* next_token(Parser* parser) {
  return ++(parser->current_token);
}

Token* peek_token(Parser* parser, int n) {
  Token* look_ahead = parser->current_token;
  for (int i = 0; i < n; i++) {
    look_ahead++;
  }
  return look_ahead;
}

void push_node(Parser* parser, NODE_TYPE type, void* node) {
  ASTNodePtr np = __MALLOC__(sizeof(ASTNode));
  np->type = type;
  np->node = node;

  parser->current_node->next = np;
  parser->current_node = parser->current_node->next;
}

void exit_parser(Parser* parser) {

  while(parser->root_node != NULL) {
    ASTNodePtr n = parser->root_node->next;
    __FREE__(parser->root_node->node);
    __FREE__(parser->root_node);
    parser->root_node = n;
  }

  __FREE__(parser);
}
