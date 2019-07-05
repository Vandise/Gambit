#ifndef __GAMBIT_SYMBOL_TABLEH
#define __GAMBIT_SYMBOL_TABLEH 1

#include "common.h"

#define enter_name_local_symtab(idp, name, r) idp = insert_symbol_table(name, r)

typedef union {
  int integer;
  float real;
  char character;
  char* stringp;
} Value;

typedef enum {
  UNDEFINED, MAIN_DEFN, CONST_DEFN, VAR_DEFN, TYPE_DEFN, FIELD_DEFN
} DEFN_KEY;

typedef enum {
  DECLARED, FORWARD
} ROUTINE_KEY;

typedef enum {
  NO_FORM,
  SCALAR_FORM, ENUM_FORM, SUBRANGE_FORM,
  ARRAY_FORM, STRUCT_FORM
} TYPE_FORM;

typedef struct GambitTypeStruct {

  TYPE_FORM form;
  int       size;

  struct GambitASTNode *type_idp;

  union {
    struct {
      struct GambitASTNode* const_idp;
      int max;
    } enumeration;

    struct {
      struct GambitTypeStruct *range_typep;
      int min, max;
    } subrange;

    struct {
      struct GambitTypeStruct *index_typep, *element_typep;
      int min_index, max_index;
      int element_count;
    } array;

    struct {
      struct GambitASTNode *field_symtab;
    } struct_type;
  } info;

} TypeStruct, *TypeStructPtr;

typedef struct {
  DEFN_KEY key;

  union {
    struct {
      Value value;
    } constant;

    struct {
      ROUTINE_KEY key;
      char *routine_name;
      int param_count;
      int total_param_size;
      int total_local_size;
      struct ASTNode *params;
      struct ASTNode *locals;
      struct ASTNode *local_symtab;
      char *code_segment;
    } routine;

    struct {
      int offset;
      struct ASTNode *record_idp;
    } data;

  } info;
} DefnStruct;

typedef struct GambitASTNode {

  struct GambitASTNode *left, *right;
  struct GambitASTNode *next;
  char *name;
  char *info;

  DefnStruct defn;
  TypeStructPtr typep;

  int level;
  int label_index;
} ASTNode, *ASTNodePtr;

extern TypeStructPtr IntegerTypePtr, RealTypePtr, BooleanTypePtr, CharTypePtr;

ASTNodePtr search_symbol_table(const char *name, ASTNodePtr nodep);
ASTNodePtr insert_symbol_table(const char *name, ASTNodePtr *nodepp);
void init_symbol_table(ASTNodePtr *root_node);
void free_symbol_table_node(ASTNodePtr nodep);
void free_ast(ASTNodePtr node);
void free_symbol_type(TypeStructPtr type);
void exit_symbol_table(ASTNodePtr node);

#endif
