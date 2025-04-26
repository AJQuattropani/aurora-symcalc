#pragma once
#include <stdio.h>

#include "types.h"
#include "vector.h"
#include "initializers.h"

enum opr_t {
  BINARY,
  UNARY,
  CONSTANT,
  IDENTITY,
};
typedef enum opr_t opr_t;

struct binary_f {
  b_opliteral op;
  f_node *left;
  f_node *right;
};

struct unary_f {
  u_opliteral op;
  f_node *in;
};

struct const_f {
  vd_literal output;
};

struct iden_f {
  argcnt_t index;
};


struct function_node {
  mString name;
  depth_t depth_index;
  priority_t priority;
  opr_t ty;
  union {
    void* __align_data;
    struct binary_f bf;
    struct unary_f uf;
    struct const_f cf; // none needed for constant
    struct iden_f xf; // identity: index of input args
  };
};

f_node *new_fnode();
void free_fobject(f_object *fun);
void free_fnode_recurse(f_node *node);
void sprint_function(gString *inp, const f_object *fun);
void fnode_str_recurse(gString *inp, const f_node *fun);


