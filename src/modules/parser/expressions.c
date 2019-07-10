#include "modules/parser/expressions.h"

TOKEN_CODE rel_op_list[] = { T_LT, T_LE, T_EQUAL, T_GE, T_GT, 0 };
TOKEN_CODE add_op_list[] = { T_PLUS, T_MINUS, 0 };
TOKEN_CODE mult_op_list[] = { T_STAR, T_SLASH, 0 };

ASTNodePtr expression(Parser* parser) {
  TOKEN_CODE op;
  ASTNodePtr node, node2 = NULL;

  simple_expression(parser);

  if (token_in_list(parser->current_token->code, rel_op_list)) {
    op = parser->current_token->code;
    next_token(parser);
    simple_expression(parser);
  }

  return node;
}

ASTNodePtr simple_expression(Parser* parser) {
  TOKEN_CODE op;
  TOKEN_CODE unary_op = T_PLUS;
  ASTNodePtr node, node2 = NULL;

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

  return node;
}

ASTNodePtr term(Parser* parser) {
  TOKEN_CODE op;
  ASTNodePtr node, node2 = NULL;

  factor(parser);

  while ( token_in_list(parser->current_token->code, mult_op_list) ) {
    op = parser->current_token->code;
    next_token(parser);
    factor(parser);
  }

  return node;
}

ASTNodePtr factor(Parser* parser) {
  ASTNodePtr node = NULL;

  switch( parser->current_token->code ) {
    case T_IDENTIFIER: {
      GetLocalNodePtr g = __MALLOC__(sizeof(GetLocalNode));
      g->identifier = parser->current_token->token_string;

      node = build_node(GET_LOCAL_NODE, g);

      next_token(parser);
      break;
    }

    case T_NUMBER: {
      LiteralNodePtr l = __MALLOC__(sizeof(LiteralNode));
      l->type = parser->current_token->literal.type;

      if (l->type == INTEGER_LIT) { l->value.integer = parser->current_token->literal.value.integer; }
      else { l->value.real = parser->current_token->literal.value.real; }

      node = build_node(LITERAL_NODE, l);

      next_token(parser);
      break;
    }

    case T_STRING: {
      LiteralNodePtr l = __MALLOC__(sizeof(LiteralNode));
      l->type = parser->current_token->literal.type;
      l->value.stringp = parser->current_token->literal.value.string;

      node = build_node(LITERAL_NODE, l);

      next_token(parser);
      break;
    }

    case T_LPAREN:
      next_token(parser);
      expression(parser);
      if (parser->current_token->code != T_RPAREN) {
        parser->errored = TRUE;
      }
      break;

    default:
      parser->errored = TRUE;
      break;
  }

  return node;
}