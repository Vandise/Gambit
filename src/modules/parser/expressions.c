#include "modules/parser/expressions.h"

/*
  precedence:
  
    left-to-right
      .
      postfix: ++, --
      +, -
      >, <, <=, >=, ==, !=

    right-to-left
      unary: +, -
      prefix: ++, --
      assignment: =
*/

TOKEN_CODE rel_op_list[] = { T_LT, T_LE, T_EQUAL, T_GE, T_GT, T_AND, T_OR, 0 };
TOKEN_CODE add_op_list[] = { T_PLUS, T_MINUS, 0 };
TOKEN_CODE mult_op_list[] = { T_STAR, T_SLASH, 0 };

ASTNodePtr expression(Parser* parser) {
  TOKEN_CODE op;
  ASTNodePtr node, left, right = NULL;

  //printf("\n\n expression: %s (%d) \n\n", parser->current_token->token_string, parser->current_token->code);

  node = simple_expression(parser);

  if (token_in_list(parser->current_token->code, rel_op_list)) {
    op = parser->current_token->code;
    left = node; node = NULL;

    next_token(parser);

    right = simple_expression(parser);

    BinaryOpNodePtr bn = __MALLOC__(sizeof(BinaryOpNode));
    bn->op = op;

    node = build_node(BINARY_OP_NODE, bn);
    node->left = left;
    node->right = right;
  }

  return node;
}

ASTNodePtr simple_expression(Parser* parser) {
  TOKEN_CODE unary_op = T_PLUS;

  ASTNodePtr node, current_node, bn_op = NULL;

  UnaryOpNodePtr unop;
  BOOLEAN saw_unary_op = FALSE;

  //printf("\n\n simple expression: %s (%d) \n\n", parser->current_token->token_string, parser->current_token->code);

  /*
    possibly make unary a factor
      then make the expression a factor
  */
  if (parser->current_token->code == T_PLUS || parser->current_token->code == T_MINUS) {
    saw_unary_op = TRUE;
    unop = __MALLOC__(sizeof(UnaryOpNode));
    unop->op = parser->current_token->code;
    next_token(parser);
  }

  if (saw_unary_op) {
    node = build_node(UNARY_OP_NODE, unop);
    node->right = term(parser);
  } else {
    node = term(parser);
  }

  //
  // 1 + 2 + 3
  //

  //
  // Literal 1 node
  //
  current_node = node;


  while( token_in_list(parser->current_token->code, add_op_list) ) {

    //printf("\n\n add_op_list: (%p) \n\n", current_node);

    //
    // it1 - current node = 1
    //          l,r = null
    //
    // it2 - current_node: l = 1, r = 2
    //
    //
    BinaryOpNodePtr b = __MALLOC__(sizeof(BinaryOpNode));
    b->op = parser->current_token->code;

    //
    // it1 - bn_op: l = 1, r = null
    //
    // it2 - bn_op: l = BinOp{ l = 1, r = 2 }
    //
    bn_op = build_node(BINARY_OP_NODE, b);
    bn_op->left = current_node;

    next_token(parser);

    //
    // it1 - bn_op: r = 2
    //
    // it2 - bn_op: r = 3, l = BinOp{ l = 1, r = 2 }
    //
    bn_op->right = term(parser);

    //
    // it1 - current_node = bn_op: l=1, r=2
    //
    // it2 - current_node = bn_op: r = 3, l = BinOp{ l = 1, r = 2 }
    //
    current_node = bn_op;
  }

  return current_node;
}

ASTNodePtr term(Parser* parser) {
  ASTNodePtr node, current_node, bn_op = NULL;
  UnaryOpNodePtr unop;

  //printf("\n\n term: %s (%d) \n\n", parser->current_token->token_string, parser->current_token->code);

  current_node = node = factor(parser);

  while ( token_in_list(parser->current_token->code, mult_op_list) ) {
    //printf("\n\n mult_op_list: (%p) \n\n", current_node);

    BinaryOpNodePtr b = __MALLOC__(sizeof(BinaryOpNode));
    b->op = parser->current_token->code;

    bn_op = build_node(BINARY_OP_NODE, b);
    bn_op->left = current_node;

    next_token(parser);

    bn_op->right = factor(parser);

    current_node = bn_op;
  }

  return current_node;
}

ASTNodePtr factor(Parser* parser) {
  ASTNodePtr node = NULL;

  //printf("\n\n factor: %s (%d) \n\n", parser->current_token->token_string, parser->current_token->code);

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
      node = expression(parser);
      if (parser->current_token->code != T_RPAREN) {
        parser->errored = TRUE;
      } else { next_token(parser); }
      break;

    default:
      break;
  }

  return node;
}