#pragma once

#include "../env/operators.h"
#include "../objects/function.h"

static __attribute__((always_inline)) inline void
update_depth_max(depth_t depth, depth_t *depth_max) {
  *depth_max = (depth ^ ((depth ^ *depth_max) & -(depth < *depth_max))) + 1;
}

f_node *differentiate_node(const f_node *, size_t);

f_node *diff_bin(const f_node *in, size_t argidx, f_attribs *attr) {
  f_node *g = (in->bf.left);
  f_node *gx = differentiate_node(in->bf.left, argidx);
  f_node *h = (in->bf.right);
  f_node *hx = differentiate_node(in->bf.right, argidx);

  f_node *fg, *fh;
  f_node *f_prime;
  if (vb_add == in->bf.op) {
    // f = g + h -> f' = g' + h'
    f_prime = new_fnode();
    *f_prime = (f_node){.name = m_from_cstr("+"), .ty = BINARY, .bf = {.op = vb_add, .left = gx, .right = hx}};
    
  } else if (vb_mul == in->bf.op) {
    fg = h;
    fh = g;
  } else if (vb_div == in->bf.op) {
    
  }

  // do in place simplification before returning
  //return chain_rule;
}

f_node *diff_un(const f_node *in, size_t argidx, f_attribs *attr) {
  f_node *g = (in->uf.in);
  f_node *dg = (in->uf.in);
  
}

f_node *differentiate_node(const f_node *in, size_t argidx, f_attribs *attr) {
  switch (in->ty) {
  case BINARY:
    return diff_bin(in, argidx, attr);
  case UNARY:
    return diff_un(in, argidx, attr);
  case CONSTANT:
    return diff_cnst(in, argidx, attr);
  case IDENTITY:
    return diff_identity(in, argidx, attr);
  }
}

int differentiate(f_object *out, const f_object *in, size_t argidx) {
  if (argidx >= in->attr.argcnt)
    return 1;

  out->root = differentiate_node(in->root, argidx, &out->attr);
  return 0;
}
