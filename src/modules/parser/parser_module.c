#include "modules/parser/parser_module.h"

Parser* init_parser(TokenArray* tokens) {
  Parser* parser = __MALLOC__(sizeof(Parser));
  parser->token_array = tokens;
  parser->errored = FALSE;
  parser->root_node = NULL;

  return parser;
}

void exit_parser(Parser* parser) {
  free_token_array(parser->token_array);
  __FREE__(parser);
}
