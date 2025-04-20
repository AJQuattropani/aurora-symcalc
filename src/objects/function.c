#include "function.h"

f_node *new_node() {
  f_node *no = (f_node *)malloc(sizeof(f_node));
  if (NULL == no) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return no;
}

Object as_fobject(f_object *fun) {
  return (Object){.fObject = *fun, .ty = FUNC};
}

void free_fobject(f_object *fun) {
  free_fnode_recurse(fun->root);
  free(fun);
}

void free_fnode_recurse(f_node *node) {
  if (NULL == node)
    return;
  free_vdliteral(&node->output);
  switch (node->ty) {
  case BINARY:
    free_bopliteral(&node->bf.op);
    free_fnode_recurse(node->bf.left);
    free_fnode_recurse(node->bf.right);
    break;
  case UNARY:
    free_uopliteral(&node->uf.op);
    free_fnode_recurse(node->uf.in);
    break;
  case CONSTANT:
    break;
  case IDENTITY:
    break;
  }
  free(node);
}

__attribute__((always_inline)) inline void
sprint_function([[maybe_unused]] gString *inp,
                [[maybe_unused]] const f_object *fun) {}
