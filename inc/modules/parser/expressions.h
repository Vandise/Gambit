#ifndef __GAMBIT_PARSER_EXPRESSIONSH
#define __GAMBIT_PARSER_EXPRESSIONSH 1

#include "common.h"
#include "shared/tokens.h"
#include "modules/parser/parser_module.h"
#include "modules/parser/ast/node.h"

void expression(Parser* parser);
void simple_expression(Parser* parser);
void term(Parser* parser);
void factor(Parser* parser);

#endif
