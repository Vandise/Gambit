#ifndef __GAMBIT_PARSER_MODULEH
#define __GAMBIT_PARSER_MODULEH 1

#include "common.h"
#include "shared/tokens.h"
#include "shared/token_array.h"
#include "modules/parser/ast/node.h"

typedef struct GambitParser {
  TokenArray* token_array;
  Token* current_token;
  ASTNodePtr root_node;
  ASTNodePtr current_node;
  BOOLEAN errored;
} Parser;

Parser* init_parser(TokenArray* tokens);
Token* next_token(Parser* parser);
Token* peek_token(Parser* parser, int n);
void push_node(Parser* parser, ASTNodePtr np);
void exit_parser(Parser* parser);

#endif
