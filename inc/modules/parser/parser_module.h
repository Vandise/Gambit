#ifndef __GAMBIT_PARSER_MODULEH
#define __GAMBIT_PARSER_MODULEH 1

#include "common.h"
#include "shared/tokens.h"
#include "shared/token_array.h"
#include "modules/parser/ast/symbol_table.h"

typedef struct GambitParser {
  TokenArray* token_array;
  ASTNodePtr* root_node;
  BOOLEAN errored;
} Parser;

Parser* init_parser(TokenArray* tokens);
void exit_parser(Parser* parser);

#endif
