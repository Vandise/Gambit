#include "modules/parser/declarations.h"

ASTNodePtr declarations(Parser* parser) {
  ASTNodePtr node = NULL;

  switch( parser->current_token->code  ) {

    case T_LET: {
      node = variable_declaration(parser);
      break;
    }

    case T_STRUCT: {
      node = struct_declaration(parser);
      break;
    }

    default: {
      break;
    }

  }

  return node;
}

TOKEN_CODE struct_prop_type_list[] = { T_IDENTIFIER, 0 };

//
// struct <Constant> do
//  <struct_properties>
// end
//
// left: identifier, right: properties tree
ASTNodePtr struct_declaration(Parser* parser) {
  ASTNodePtr currentNode = NULL;
  ASTNodePtr root = NULL;

  if((next_token(parser))->code != T_CONSTANT) {
    parser->errored = TRUE;
    return root;
  }

  StructDeclarationNodePtr structdecl = __MALLOC__(sizeof(StructDeclarationNode));
  root = build_node(STRUCT_DECLARATION_NODE, structdecl);
  currentNode = root;

  LiteralNodePtr l = __MALLOC__(sizeof(LiteralNode));
  l->type = STRING_LIT;

  l->value.stringp = parser->current_token->token_string;
  currentNode->left = build_node(LITERAL_NODE, l);

  if((next_token(parser))->code != T_DO) {
    parser->errored = TRUE;
    return root;
  }

  if((next_token(parser))->code != T_NEWLINE) {
    parser->errored = TRUE;
    return root;
  }

  next_token(parser);

  //
  // identifier : factor
  //
  while(token_in_list(parser->current_token->code, struct_prop_type_list)) {
    StructPropertyNodePtr structProperty = __MALLOC__(sizeof(StructPropertyNode));
    structProperty->body = NULL;

    currentNode->right = build_node(STRUCT_PROPERTY_NODE, structProperty);
    currentNode = currentNode->right;

    LiteralNodePtr l = __MALLOC__(sizeof(LiteralNode));
    l->type = STRING_LIT;
    l->asParam = TRUE;

    l->value.stringp = parser->current_token->token_string;
    currentNode->left = build_node(LITERAL_NODE, l);

    if((next_token(parser))->code != T_COLON) {
      parser->errored = TRUE;
      return root;
    }

    // factor
    next_token(parser);

    structProperty->body = factor(parser);

    if(parser->current_token->code != T_NEWLINE) {
      parser->errored = TRUE;
      return root;
    }

    // identifier or end
    next_token(parser);
  }

  if(parser->current_token->code != T_END) {
    parser->errored = TRUE;
    return root;
  }

  next_token(parser);

  return root;
}

//
// let( [ T_PARAM ] T_IDENTIFIER [ T_COLON T_CONSTANT|T_LITERAL ] [T_COMMA] ...) = <expression>
//

TOKEN_CODE var_definition_type_list[] = { T_DOLLARSIGN, T_IDENTIFIER, 0 };

static void insert_declaration_chain(ASTNodePtr root, ASTNodePtr node) {
  //printf("\n variable_declaration - inserting into node chain %p %p \n", root, node);
  if (root == node) { return; }

  ASTNodePtr current_element = root;
  while(current_element->next != NULL) {
    current_element = current_element->next;
  }

  current_element->next = node;
}

ASTNodePtr variable_declaration(Parser* parser) {
  ASTNodePtr node = NULL;
  ASTNodePtr root = NULL;

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

    //printf("\n variable_declaration - while in list \n");

    saw_dollarsign = FALSE;
    saw_colon = FALSE;

    if (parser->current_token->code == T_DOLLARSIGN) {
      //printf("\n variable_declaration - saw_dollarsign \n");
      saw_dollarsign = TRUE;
      next_token(parser);
    }

    if (parser->current_token->code != T_IDENTIFIER) {
      //printf("\n variable_declaration - no T_IDENTIFIER \n");
      parser->errored = TRUE;
      return NULL;
    }

    VariableDeclarationNodePtr var = __MALLOC__(sizeof(VariableDeclarationNode));
    var->definition.type = DEFINITION_VARIABLE;

    node = build_node(VARIABLE_DECLARATION_NODE, var);

    if (root == NULL) {
      root = node;
      //printf("\n variable_declaration - no root node, setting ( %p )\n", root);
    }

    //
    // variable name
    //
    LiteralNodePtr l = __MALLOC__(sizeof(LiteralNode));
    l->type = STRING_LIT;

    l->value.stringp = parser->current_token->token_string;
    node->left = build_node(LITERAL_NODE, l);

    //printf("\n variable_declaration - build identifier %s \n", l->value.stringp);

    //
    // colon : T_CONSTANT|T_LITERAL 
    //
    next_token(parser);

    if (parser->current_token->code == T_COLON) {

      //printf("\n variable_declaration - saw_colon \n");

      saw_colon = TRUE;
      next_token(parser);

      //printf("\n variable_declaration - saw_colon : token( %d ) \n", parser->current_token->code);

      switch(parser->current_token->code) {
        case T_CONSTANT: {
          //printf("\n variable_declaration - saw_colon : T_CONSTANT \n");
          var->definition.info.variable.value.stringp = parser->current_token->token_string;
          var->definition.info.variable.type = VARIABLE_MATCH_CONSTANT;
          var->definition.info.variable.value_type = STRING_LIT;
          break;
        }

        case T_NUMBER: {
          if (parser->current_token->literal.type == INTEGER_LIT) {
            //printf("\n variable_declaration - saw_colon : INTEGER_LIT \n");
            var->definition.info.variable.value.integer = parser->current_token->literal.value.integer;
            var->definition.info.variable.type = VARIABLE_MATCH_VALUE;
            var->definition.info.variable.value_type = INTEGER_LIT;
          } else {
            //printf("\n variable_declaration - saw_colon : REAL_LIT \n");
            var->definition.info.variable.value.real = parser->current_token->literal.value.real;
            var->definition.info.variable.type = VARIABLE_MATCH_VALUE;
            var->definition.info.variable.value_type = REAL_LIT;
          }
          break;
        }

        case T_STRING: {
          //printf("\n variable_declaration - saw_colon : STRING_LIT \n");
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

      next_token(parser);

    } else {
      // optimization
      if (saw_dollarsign) {
        var->definition.info.variable.type = VARIABLE_MATCH_ALL;
      } else {
        var->definition.info.variable.type = VARIABLE_SIMPLE_ASSIGN;
      }
    }

    //printf("\n variable_declaration - check T_COMMA %d \n", parser->current_token->code);

    //
    // comma or right paren
    //
    if (parser->current_token->code == T_COMMA) {
      //printf("\n variable_declaration - found T_COMMA \n");
      next_token(parser); // get next in var_definition_type_list
    }

    //
    // essentially this just splits a declaration let(a, b) = <expression>
    // into two different assignment nodes
    // this allows easier pattern matching on objects
    //
    insert_declaration_chain(root, node);
  }

  //printf("\n variable_declaration - check  T_RPAREN \n");

  //
  // todo: memory leak if errored
  //
  if (parser->current_token->code != T_RPAREN) {
    parser->errored = TRUE;
    return NULL;
  }

  //printf("\n variable_declaration - check  T_EQUAL \n");

  if ((next_token(parser))->code != T_EQUAL) {
    parser->errored = TRUE;
    return NULL;
  }

  //
  // jump to first expression token
  //
  next_token(parser);

  //printf("\n variable_declaration - set root->right  %p \n", root);

  root->right = expression(parser);

  node = root->next;
  //printf("\n variable_declaration - root next  %p \n", node);
  while(node != NULL) {
    node->right = root->right;
    //printf("\n variable_declaration - setting next node\n");
    node = node->next;
  }

  return root;
}