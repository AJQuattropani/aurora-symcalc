#include "function.h"

f_node *new_fnode() {
  f_node *no = (f_node *)malloc(sizeof(f_node));
  if (NULL == no) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return no;
}

f_object copy_fobject(const f_object *other) {
  f_object copy = *other;
  copy.root = copy_fnode_recurse(other->root);
  return copy;
}

f_node *copy_fnode_recurse(const f_node *ref) {
  f_node *dup = new_fnode();
  switch (ref->ty) {
  case BINARY:
    dup->bf.op = ref->bf.op;
    dup->bf.left = copy_fnode_recurse(ref->bf.left);
    dup->bf.right = copy_fnode_recurse(ref->bf.right);
    break;
  case UNARY:
    dup->uf.op = ref->uf.op;
    dup->uf.in = copy_fnode_recurse(ref->uf.in);
    break;
  case CONSTANT:
    dup->cf.output = copy_vdliteral(&ref->cf.output);
    break;
  case IDENTITY:
    dup->xf.index = ref->xf.index;
    break;
  }
  dup->ty = ref->ty;
  dup->priority = ref->priority;
  dup->depth_index = ref->depth_index;
  dup->name = m_from_copy(ref->name);
  return dup;
}

void free_fobject(f_object *fun) {
  free_fnode_recurse(fun->root);
  *fun = (f_object){0};
}

void free_fnode_recurse(f_node *node) {
  if (NULL == node)
    return;
  switch (node->ty) {
  case BINARY:
    m_deletestr(&node->name);
    free_bopliteral(&node->bf.op);
    free_fnode_recurse(node->bf.left);
    free_fnode_recurse(node->bf.right);
    break;
  case UNARY:
    m_deletestr(&node->name);
    free_uopliteral(&node->uf.op);
    free_fnode_recurse(node->uf.in);
    break;
  case CONSTANT:
    m_deletestr(&node->name);
    free_vdliteral(&node->cf.output);
    break;
  case IDENTITY:
    m_deletestr(&node->name);
    break;
  }
  free(node);
}

void fnode_str_recurse(gString *inp, const f_node *fun) {
  switch (fun->ty) {
  case BINARY: {
    if (fun->priority < fun->bf.left->priority) {
      g_append_back_c(inp, "(");
      fnode_str_recurse(inp, fun->bf.left);
      g_append_back_c(inp, ")");
    } else {
      fnode_str_recurse(inp, fun->bf.left);
    }
    g_append_back(inp, fun->name.cstring, fun->name.size);
    if (fun->priority < fun->bf.right->priority) {
      g_append_back_c(inp, "(");
      fnode_str_recurse(inp, fun->bf.right);
      g_append_back_c(inp, ")");
    } else {
      fnode_str_recurse(inp, fun->bf.right);
    }
    return;
  }
  case UNARY: {
    g_append_back(inp, fun->name.cstring, fun->name.size);
    g_append_back_c(inp, "(");
    fnode_str_recurse(inp, fun->uf.in);
    g_append_back_c(inp, ")");
    return;
  }
  case CONSTANT: {
    g_append_back(inp, fun->name.cstring, fun->name.size);
    return;
  }
  case IDENTITY: {
    g_append_back(inp, fun->name.cstring, fun->name.size);
    return;
  }
  }
}

__attribute__((always_inline)) inline void
sprint_function([[maybe_unused]] gString *inp,
                [[maybe_unused]] const f_object *fun) {
  // space to implement function-to-string conversion
  g_append_back_c(inp, "(");

  if (0 < fun->attr.argcnt) {
    g_append_back_c(inp, "x");
    argcnt_t i = 0;
    const int b_size = 7;
    char numbuff[b_size];
    int n;
    while (1) {
      n = snprintf(numbuff, b_size, "%d", i);
      if (n >= b_size || 0 >= n) {
        fprintf(stderr, "snprintf failed in %s", __func__);
        exit(1);
      }
      g_append_back(inp, numbuff, n);
      memset(numbuff, '\0', b_size);
      if (++i >= fun->attr.argcnt)
        break;
      g_append_back_c(inp, ",x");
    }
  }
  g_append_back_c(inp, ")");

  if (NULL == fun->root)
    return;
  g_append_back_c(inp, "=");

  fnode_str_recurse(inp, fun->root);
}

void update_depth(f_node *curr, depth_t depth, depth_t *tot_depth) {
  update_depth_max(depth, tot_depth);
  curr->depth_index = depth;
  switch (curr->ty) {
  case BINARY:
    update_depth(curr->bf.left, depth, tot_depth);
    update_depth(curr->bf.right, depth + 1, tot_depth);
    break;
  case UNARY:
    update_depth(curr->uf.in, depth, tot_depth);
    break;
  case CONSTANT:
  case IDENTITY:
    break;
  }
}
