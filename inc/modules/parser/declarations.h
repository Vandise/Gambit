#ifndef __GAMBIT_PARSER_MODULE_DECLARATIONSH
#define __GAMBIT_PARSER_MODULE_DECLARATIONSH 1

#include "shared/tokens.h"
#include "modules/parser/parser_module.h"
#include "modules/parser/expressions.h"

ASTNodePtr declarations(Parser* parser);
ASTNodePtr variable_declaration(Parser* parser);

#endif
