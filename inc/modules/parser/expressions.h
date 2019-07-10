#ifndef __GAMBIT_PARSER_MODULE_EXPRESSIONSH
#define __GAMBIT_PARSER_MODULE_EXPRESSIONSH 1

#include "shared/tokens.h"
#include "modules/parser/parser_module.h"

ASTNodePtr expression(Parser* parser);
ASTNodePtr simple_expression(Parser* parser);
ASTNodePtr term(Parser* parser);
ASTNodePtr factor(Parser* parser);

#endif
