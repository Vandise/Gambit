#include "modules/parser/expressions.h"

TOKEN_CODE rel_op_list[] = { T_LT, T_LE, T_EQUAL, T_GE, T_GT, 0 };
TOKEN_CODE add_op_list[] = { T_PLUS, T_MINUS, 0 };
TOKEN_CODE mult_op_list[] = { T_STAR, T_SLASH, 0 };

void expression(Parser* parser) {
  TOKEN_CODE op;

  simple_expression(parser);

  if (token_in_list(parser->current_token->code, rel_op_list)) {
    op = parser->current_token->code;
    next_token(parser);
    simple_expression(parser);
  }
}

void simple_expression(Parser* parser) {
  TOKEN_CODE op;
  TOKEN_CODE unary_op = T_PLUS;

  if (parser->current_token->code == T_PLUS || parser->current_token->code == T_MINUS) {
    unary_op = parser->current_token->code;
    next_token(parser);
  }

  term(parser);

  while( token_in_list(parser->current_token->code, add_op_list) ) {
    op = parser->current_token->code;
    next_token(parser);
    term(parser);
  }
}

void term(Parser* parser) {
  TOKEN_CODE op;

  factor(parser);

  while ( token_in_list(parser->current_token->code, mult_op_list) ) {
    op = parser->current_token->code;
    next_token(parser);
    factor(parser);
  }
}

void factor(Parser* parser) {
  switch( parser->current_token->code ) {
    case T_IDENTIFIER:
      next_token(parser);
      break;
    case T_NUMBER:
      next_token(parser);
      break;
    case T_STRING:
      next_token(parser);
      break;
    case T_LPAREN:
      next_token(parser);
      expression(parser);
      if (parser->current_token->code != T_RPAREN) {
        parser->errored = TRUE;
        return;
      }
      break;
    default:
      parser->errored = TRUE;
      break;
  }
}