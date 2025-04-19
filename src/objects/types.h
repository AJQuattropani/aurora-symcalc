#pragma once
#include <stdlib.h>
#include "../strings/vstring.h"

/*
 * Forward definition of the structure in which
 * full runtime context is stored. This includes
 * runtime status, current file reading, and
 * the hash map containing all environment objects.
 */
struct environment;
typedef struct environment env;


struct token;
typedef struct token token;

struct token_array;
typedef struct token_array token_array;

/*
 * List of cursory types an object can have.
 * Switches and checks of these values allows for
 * dynamic reinterpretation.
 */
enum object_type {
  NONE = 0,
  VECTOR,
  BOPER,
  UOPER,
  CONTEXT,
  FUNC,
};
typedef enum object_type obj_t;

/*
 * ENVIRONMENT OBJECT DEFINITION:
 * may either be a pointer to instructions, a literal, or
 * an object like a function
 */
struct vector_literal;
typedef struct vector_literal vd_literal;
struct vector_literal {
  double* data;
  size_t size;
};
typedef void(*context_modifier)(env* context, const token_array* args);
typedef context_modifier mf_context; // generic type for functions that modify the runtime environment
typedef int(*binary_operation_literal)(vd_literal *o, const vd_literal *l, const vd_literal *r);
typedef binary_operation_literal b_opliteral; // generic type for operations on two vectors
typedef int(*unary_operation_literal)(vd_literal *o, const vd_literal *i);
typedef unary_operation_literal u_opliteral; // generic type for operations on one vector

struct function_node;
typedef struct function_node f_node;
struct function_object;
typedef struct function_object f_object;
struct function_object {
  f_node* root;
  size_t argcnt;
};

enum opr_t {
  BINARY,
  UNARY,
  CONSTANT,
  IDENTITY
};
typedef enum opr_t opr_t;

struct function_node {
  union {
    struct {
      b_opliteral op;
      f_node* left;
      f_node* right;
    } bf;
    struct {
      u_opliteral op;
      f_node* in;
    } uf;
    //struct { // none needed for constant
    //} cf;
    // identity: index of input args
    struct {
      size_t index;
    } xf;
  };
  // storage buffer for output of operation
  // for constant, this is a static vector
  vd_literal output;
  opr_t ty;
};

struct object;
typedef struct object Object;
struct object {
  union {
    vd_literal vLiteral;
    mf_context mContext;
    b_opliteral bOperation;
    u_opliteral uOperation;
    f_object fObject;
  };
  obj_t ty;
};







