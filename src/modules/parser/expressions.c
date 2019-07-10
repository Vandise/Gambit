#include "modules/parser/expressions.h"

void expression(Parser* parser) {
  simple_expression(parser);
}

void simple_expression(Parser* parser) {
  term(parser);
}

void term(Parser* parser) {
  factor(parser);
}

void factor(Parser* parser) {
  switch(parser->current_token->code) {

    case T_STRING: {
      LiteralNodePtr np = __MALLOC__(sizeof(LiteralNode));
      np->type = STRING_LIT;
      np->value.stringp = parser->current_token->literal.value.string;
      push_node(parser, LITERAL_NODE, np);
      break;
    }

    default:
      break;
  }
}