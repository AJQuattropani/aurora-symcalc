#pragma once
#include <stdio.h>

#include "types.h"
#include "vector.h"
#include "initializers.h"

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
    //struct { // none needed for constant
    //} cf;
    // identity: index of input args
    struct {
      unsigned short index;
    } xf;
  };
  priority_t priority;
  opr_t ty;
  // storage buffer for output of operation
  // for constant, this is a static vector
  vd_literal output;
};

f_node *new_fnode();
Object as_fobject(f_object *fun);
void free_fobject(f_object *fun);
void free_fnode_recurse(f_node *node);
void sprint_function(gString *inp, const f_object *fun);
void fnode_str_recurse(gString *inp, const f_node *fun);


