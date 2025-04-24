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

struct function_node {
  mString name;
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
    struct { // none needed for constant
      vd_literal output;
    } cf;
    // identity: index of input args
    struct {
      argcnt_t index;
    } xf;
  };
  depth_t depth_index;
  priority_t priority;
  opr_t ty;
};

f_node *new_fnode();
void free_fobject(f_object *fun);
void free_fnode_recurse(f_node *node);
void sprint_function(gString *inp, const f_object *fun);
void fnode_str_recurse(gString *inp, const f_node *fun);


