#include "modules/parser/statements.h"

ASTNodePtr statement(Parser* parser) {
  ASTNodePtr node = NULL;

  switch( parser->current_token->code  ) {

    case T_IDENTIFIER: {
      node = assignment_statement(parser);
      break;
    }

    default: {
      break;
    }

  }

  return node;
}

/*
  T_IDENTIFIER = <expression>
*/
ASTNodePtr assignment_statement(Parser* parser) {
  //
  // a GET_LOCAL node
  //
  if ( (peek_token(parser, 1))->code != T_EQUAL) {
    return NULL;
  }

  ASTNodePtr node, left, right;

  LiteralNodePtr l = __MALLOC__(sizeof(LiteralNode));
  l->type = parser->current_token->literal.type;
  l->value.stringp = parser->current_token->literal.value.string;

  left = build_node(LITERAL_NODE, l);

  next_token(parser);

  if (parser->current_token->code != T_EQUAL) {
    parser->errored = TRUE;
    return NULL;
  }

  next_token(parser);

  right = expression(parser);

  AssignmentNodePtr an = __MALLOC__(sizeof(AssignmentNode));

  //
  // todo: type info
  //
  node = build_node(ASSIGNMENT_NODE, an);
  node->left = left;
  node->right = right;

  return node;
}