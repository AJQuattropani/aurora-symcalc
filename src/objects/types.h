#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
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

struct object;
typedef struct object Object;

/*
 * List of cursory types an object can have.
 * Switches and checks of these values allows for
 * dynamic reinterpretation.
 */
enum object_type {
  NONE = 0,
  TEMP,
  SYNTAX_RIGHT,
  VECTOR,
  FUNC,
  BOPER,
  UOPER,
  CONTEXT,
  READER,
};
typedef enum object_type obj_t;

/*
 * ENVIRONMENT OBJECT DEFINITION:
 * may either be a pointer to instructions, a literal, or
 * an object like a function
 */
struct vector_literal;
typedef struct vector_literal vd_literal;
typedef int64_t vector_size_t;
#define MAX_SIZE INT64_MAX
#define SCALAR INT64_MIN
struct vector_literal {
  double* data;
  vector_size_t size;
};
typedef void(*context_modifier)(env *context, const token_array *args);
typedef context_modifier mf_context; // generic type for functions that modify the runtime environment

typedef int(*binary_operation_literal)(vd_literal *o, const vd_literal *l, const vd_literal *r);
typedef binary_operation_literal b_opliteral; // generic type for operations on two vectors

typedef int(*unary_operation_literal)(vd_literal *o, const vd_literal *i);
typedef unary_operation_literal u_opliteral; // generic type for operations on one vector

typedef void(*reader_macro)(Object *obj, const token_array *args);
typedef reader_macro r_macro;

struct function_node;
typedef struct function_node f_node;
struct function_object;
typedef struct function_object f_object;

typedef unsigned short argcnt_t; 

struct function_object {
  f_node* root;
  argcnt_t argcnt;
  union {
    struct {
      vd_literal left;
      vd_literal right;
    } bf;
    struct {
      vd_literal in;
    } uf;
    vd_literal out;
  } cache;
};


typedef uint16_t priority_t;
#define PRIORITY_MAX USHRT_MAX

struct object {
  union {
    vd_literal vLiteral;
    mf_context mContext;
    b_opliteral bOperation;
    u_opliteral uOperation;
    f_object fObject;
    r_macro reader;
    int64_t other;
  };
  obj_t ty;
  priority_t priority;
};







