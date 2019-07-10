#ifndef __GAMBIT_PARSER_MODULE_EXPRESSIONSH
#define __GAMBIT_PARSER_MODULE_EXPRESSIONSH 1

#include "shared/tokens.h"
#include "modules/parser/parser_module.h"

void expression(Parser* parser);
void simple_expression(Parser* parser);
void term(Parser* parser);
void factor(Parser* parser);

#endif
