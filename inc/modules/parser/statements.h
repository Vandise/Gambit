#ifndef __GAMBIT_PARSER_MODULE_STATEMENTSH
#define __GAMBIT_PARSER_MODULE_STATEMENTSH 1

#include "shared/tokens.h"
#include "modules/parser/parser_module.h"
#include "modules/parser/expressions.h"

ASTNodePtr statement(Parser* parser);
ASTNodePtr assignment_statement(Parser* parser);

#endif
