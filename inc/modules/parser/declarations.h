#ifndef __GAMBIT_PARSER_DECLARATIONSH
#define __GAMBIT_PARSER_DECLARATIONSH 1

#include "common.h"
#include "shared/tokens.h"
#include "modules/parser/parser_module.h"
#include "modules/parser/ast/node.h"

void declarations(Parser* parser);
void variable_declaration(Parser* parser);

#endif
