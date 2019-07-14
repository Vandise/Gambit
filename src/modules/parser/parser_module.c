#include "modules/parser/parser_module.h"
#include "modules/parser/declarations.h"
#include "modules/parser/statements.h"
#include "modules/parser/expressions.h"
#include "modules/parser/terminators.h"

Parser* init_parser(TokenArray* tokens) {
  Parser* parser = __MALLOC__(sizeof(Parser));
  parser->token_array = tokens;
  parser->errored = FALSE;

  parser->root_node = __MALLOC__(sizeof(ASTNode));
  parser->root_node->type = NOOP_NODE;
  parser->root_node->node = __MALLOC__(sizeof(NOOPNode));
  parser->root_node->next = NULL;
  parser->root_node->left = NULL;
  parser->root_node->right = NULL;
  parser->current_node = parser->root_node;

  parser->current_token = parser->token_array->array;

  return parser;
}

Token* next_token(Parser* parser) {
  return ++(parser->current_token);
}

Token* peek_token(Parser* parser, int n) {
  Token* look_ahead = parser->current_token;
  for (int i = 0; i < n; i++) {
    look_ahead++;
  }
  return look_ahead;
}

void push_node(Parser* parser, ASTNodePtr np) {
  parser->current_node->next = np;
  parser->current_node = parser->current_node->next;
}

static void increment_current_node(Parser* parser) {
  parser->current_node = parser->current_node->next;
}

/*

  root:
    T_END_OF_FILE
    | expressions T_END_OF_FILE

  expressions:
    expression
    | expressions terminator expression
    | expressions terminator

  expression:
    declarations
    | statement
    | expression

*/
void parse(Parser* parser) {
  do {
    printf("Parser -- parse, current token: ( %d ) \n", parser->current_token->code);

    if ( (parser->current_node->next = declarations(parser)) != NULL ) {
      increment_current_node(parser);
      continue;
    }

    if ( (parser->current_node->next = statement(parser)) != NULL ) {
      increment_current_node(parser);
      continue;
    }

    if ( (parser->current_node->next = expression(parser)) != NULL ) {
      increment_current_node(parser);
      continue;
    }

    if ( (parser->current_node->next = terminator(parser)) != NULL ) {
      increment_current_node(parser);
      continue;
    }

    printf("Parser -- parse end of loop, current token: ( %d ) \n", parser->current_token->code);

  } while (parser->current_token->code != T_END_OF_FILE);

  printf("Parser -- parse end, current token: ( %d ) \n", parser->current_token->code);
}

void exit_parser(Parser* parser) {

  while(parser->root_node != NULL) {
    ASTNodePtr n = parser->root_node->next;
    __FREE__(parser->root_node->node);
    __FREE__(parser->root_node);
    parser->root_node = n;
  }

  __FREE__(parser);
}
