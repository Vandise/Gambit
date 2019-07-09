#include "modules/parser/parser_module.h"

Parser* init_parser(TokenArray* tokens) {
  Parser* parser = __MALLOC__(sizeof(Parser));
  parser->token_array = tokens;
  parser->errored = FALSE;
  parser->root_node = NULL;

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

void exit_parser(Parser* parser) {
  free_token_array(parser->token_array);
  __FREE__(parser);
}
