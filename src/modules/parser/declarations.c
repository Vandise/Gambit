#include "modules/parser/declarations.h"

ASTNodePtr declarations(Parser* parser) {
  ASTNodePtr node = NULL;

  switch( parser->current_token->code  ) {

    case T_LET: {
      node = variable_declaration(parser);
      break;
    }

    default: {
      break;
    }

  }

  return node;
}

//
// let( [ T_PARAM ] T_IDENTIFIER [ T_COLON T_CONSTANT|T_LITERAL ] [T_COMMA] ...) = <expression>
//

TOKEN_CODE var_definition_type_list[] = { T_DOLLARSIGN, T_IDENTIFIER, 0 };

ASTNodePtr variable_declaration(Parser* parser) {
  ASTNodePtr node = NULL;
  BOOLEAN saw_dollarsign = FALSE;
  BOOLEAN saw_colon = FALSE;

  if((next_token(parser))->code != T_LPAREN) {
    parser->errored = TRUE;
    return node;
  } else { next_token(parser); }

  //
  // iterate over each identifier
  //
  while(token_in_list(parser->current_token->code, var_definition_type_list)) {
    saw_dollarsign = FALSE;
    saw_colon = FALSE;

    if (parser->current_token->code == T_DOLLARSIGN) {
      saw_dollarsign = TRUE;
      next_token(parser);
    }

    if (parser->current_token->code != T_IDENTIFIER) {
      parser->errored = TRUE;
      return NULL;
    }

    //
    // variable name
    //
    LiteralNodePtr l = __MALLOC__(sizeof(LiteralNode));
    l->type = STRING_LIT;

    l->value.stringp = parser->current_token->token_string;

    VariableDeclarationNodePtr var = __MALLOC__(sizeof(VariableDeclarationNode));
    var->definition.type = DEFINITION_VARIABLE;

    node = build_node(VARIABLE_DECLARATION_NODE, var);
    node->left = build_node(LITERAL_NODE, l);

    //
    // colon : T_CONSTANT|T_LITERAL 
    //
    next_token(parser);

    //
    // todo: create variable definition chain
    //        root = node if root == null
    //        current_node = var->next
    //
    if (parser->current_token->code == T_COLON) {
      saw_colon = TRUE;
      next_token(parser);

      switch(parser->current_token->code) {
        case T_CONSTANT: {
          var->definition.info.variable.value.stringp = parser->current_token->literal.value.string;
          var->definition.info.variable.type = VARIABLE_MATCH_CONSTANT;
          var->definition.info.variable.value_type = STRING_LIT;
          break;
        }

        case T_NUMBER: {
          if (parser->current_token->literal.type == INTEGER_LIT) {
            var->definition.info.variable.value.integer = parser->current_token->literal.value.integer;
            var->definition.info.variable.type = VARIABLE_MATCH_VALUE;
            var->definition.info.variable.value_type = INTEGER_LIT;
          } else {
            var->definition.info.variable.value.real = parser->current_token->literal.value.real;
            var->definition.info.variable.type = VARIABLE_MATCH_VALUE;
            var->definition.info.variable.value_type = REAL_LIT;
          }
          break;
        }

        case T_STRING: {
          var->definition.info.variable.value.stringp = parser->current_token->literal.value.string;
          var->definition.info.variable.type = VARIABLE_MATCH_VALUE;
          var->definition.info.variable.value_type = STRING_LIT;
          break;
        }

        default: {
          parser->errored = TRUE;
          break;
        }
      }
    } else {
      // optimization
      if (saw_dollarsign) {
        var->definition.info.variable.type = VARIABLE_MATCH_ALL;
      } else {
        var->definition.info.variable.type = VARIABLE_SIMPLE_ASSIGN;
      }
    }

    //
    // comma or right paren
    //
    if ((peek_token(parser, 1))->code == T_COMMA) {
      next_token(parser); // get comma
      next_token(parser); // get next in var_definition_type_list
    }
  }

  //
  // todo: memory leak if errored
  //
  if (parser->current_token->code != T_RPAREN) {
    parser->errored = TRUE;
    return NULL;
  }

  if ((next_token(parser))->code != T_EQUAL) {
    parser->errored = TRUE;
    return NULL;
  }

  //
  // jump to first expression token
  //
  next_token(parser);

  node->right = expression(parser);

  return node;
}