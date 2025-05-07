#include "simplify.h"

//-------------------------------------------------------------------//

int handle_binary_cleanup(vd_literal * restrict inp_args, f_node * restrict curr, depth_t depth,
                          f_attribs * restrict attr) {
  int is_left_const =
      simplify_cleanup_imp(inp_args, curr->bf.left, depth, attr);
  int is_right_const =
      simplify_cleanup_imp(inp_args, curr->bf.right, depth + 1, attr);

  if (is_left_const) {
    if (is_right_const)
      return 1;

    vd_literal left_consteval = output_eval(depth, curr->bf.left, attr->depth,
                                            inp_args, attr->out_size);
    priority_t p = curr->bf.right->priority;
    free_fnode_recurse(curr->bf.left);
    curr->bf.left = new_fnode();
    gString name = g_from_capacity(10);
    sprint_vector(&name, &left_consteval);
    *curr->bf.left =
        (f_node){.name = {.cstring = name.cstring, .size = name.size},
                 .priority = p,
                 .depth_index = depth,
                 .ty = CONSTANT,
                 .cf = {.output = left_consteval}};
#define TOLERANCE 0.0000000001
    if (curr->bf.op == vb_pow) {
      if (is_num(&curr->bf.left->cf.output, 0.0, TOLERANCE) ||
          is_num(&curr->bf.left->cf.output, 1.0, TOLERANCE)) {
        free_fnode_recurse(curr->bf.right);
        f_node *left = curr->bf.left;
        m_deletestr(&curr->name);
        *curr = *left;
        free(left);
        return 1;
      }
    }
    if (curr->bf.op == vb_mul) {
      if (is_num(&curr->bf.left->cf.output, 0.0, TOLERANCE)) {
        free_fnode_recurse(curr->bf.right);
        f_node *left = curr->bf.left;
        m_deletestr(&curr->name);
        *curr = *left;
        free(left);
        return 1;
      } else if (is_num(&curr->bf.left->cf.output, 1.0, TOLERANCE)) {
        free_fnode_recurse(curr->bf.left);
        f_node *right = curr->bf.right;
        m_deletestr(&curr->name);
        *curr = *right;
        free(right);
        return 0;
      }
    }
    if (curr->bf.op == vb_add) {
      if (is_num(&curr->bf.left->cf.output, 0.0, TOLERANCE)) {
        free_fnode_recurse(curr->bf.left);
        f_node *right = curr->bf.right;
        m_deletestr(&curr->name);
        *curr = *right;
        free(right);
        return 0;
      }
    }
  }
  if (is_right_const) {
    vd_literal right_consteval = output_eval(
        depth + 1, curr->bf.right, attr->depth, inp_args, attr->out_size);
    priority_t p = curr->bf.right->priority;
    free_fnode_recurse(curr->bf.right);
    curr->bf.right = new_fnode();
    gString name = g_from_capacity(10);
    sprint_vector(&name, &right_consteval);
    *curr->bf.right =
        (f_node){.name = {.cstring = name.cstring, .size = name.size},
                 .priority = p,
                 .depth_index = depth + 1,
                 .ty = CONSTANT,
                 .cf = {.output = right_consteval}};
    if (curr->bf.op == vb_pow) {
      if (is_num(&curr->bf.right->cf.output, 0.0, TOLERANCE)) {
        free_fnode_recurse(curr->bf.left);
        f_node *right = curr->bf.right;
        m_deletestr(&curr->name);
        *curr = *right;
        curr->depth_index = depth;
        free(right);
        vd_literal one = make_scalar(1.0);
        vu_set(&curr->cf.output, &one);
        free_vdliteral(&one);
        return 1;
      } else if (is_num(&curr->bf.right->cf.output, 1.0, 0)) {
        free_fnode_recurse(curr->bf.right);
        f_node *left = curr->bf.left;
        m_deletestr(&curr->name);
        *curr = *left;
        curr->depth_index = depth;
        free(left);
        return 0;
      }
    }
    if (curr->bf.op == vb_mul) {
      if (is_num(&curr->bf.right->cf.output, 0.0, TOLERANCE)) {
        free_fnode_recurse(curr->bf.left);
        f_node *right = curr->bf.right;
        m_deletestr(&curr->name);
        *curr = *right;
        curr->depth_index = depth;
        free(right);
        return 1;
      } else if (is_num(&curr->bf.right->cf.output, 1.0, 0)) {
        free_fnode_recurse(curr->bf.left);
        f_node *left = curr->bf.left;
        m_deletestr(&curr->name);
        *curr = *left;
        curr->depth_index = depth;
        free(left);
        return 0;
      }
    }
    if (curr->bf.op == vb_add) {
      if (is_num(&curr->bf.right->cf.output, 0.0, TOLERANCE)) {
        free_fnode_recurse(curr->bf.right);
        f_node *left = curr->bf.left;
        m_deletestr(&curr->name);
        *curr = *left;
        curr->depth_index = depth;
        free(left);
        return 0;
      }
    }
  }
  update_depth_max(depth + 1 * !(is_left_const || is_right_const),
                   &attr->depth);
  return 0;
}

// returns 1 if the node is consteval
int simplify_cleanup_imp(vd_literal * restrict inp_args, f_node * restrict curr, depth_t depth,
                         f_attribs * restrict attr) {
  curr->depth_index = depth;
  switch (curr->ty) {
  case BINARY:
    return handle_binary_cleanup(inp_args, curr, depth, attr);
  case UNARY:
    if (simplify_cleanup_imp(inp_args, curr->uf.in, depth, attr))
      return 1;
    return 0;
  case CONSTANT:
    return 1;
  case IDENTITY:
    return 0;
  }
  __UNREACHABLE_BRANCH
}

/*
* POINTERS CANNOT BE THE SAME.
*/
int sort_consteval(const b_opliteral cmp, f_node ** restrict cnst_swap, f_node ** restrict curr) {
  switch ((*curr)->ty) {
  case BINARY: {
    if (cmp == (*curr)->bf.op) {
      if (sort_consteval(cmp, cnst_swap, &(*curr)->bf.left))
        return 1;
      if (sort_consteval(cmp, cnst_swap, &(*curr)->bf.right))
        return 1;
      return 0;
    }
    f_node *temp = *cnst_swap;
    *cnst_swap = *curr;
    *curr = temp;
    return 1;
  }
  case UNARY: {
    f_node *temp = *cnst_swap;
    *cnst_swap = *curr;
    *curr = temp;
    return 1;
  }
  case CONSTANT: {
    return 0;
  }
  case IDENTITY: {
    f_node *temp = *cnst_swap;
    *cnst_swap = *curr;
    *curr = temp;
    return 1;
  }
  }
  fprintf(stderr, "[FATAL] Reached end of control statement in %s.\n",
          __func__);
  exit(1);
}

int reorder_cleanup_imp(f_node * restrict curr) {
// #define A
#ifndef A
  switch (curr->ty) {
  case BINARY: {
    int left_const = reorder_cleanup_imp(curr->bf.left);
    int right_const = reorder_cleanup_imp(curr->bf.right);
    if (left_const && right_const)
      return 1;
    if (curr->bf.op == vb_mul || curr->bf.op == vb_add) {
      if (right_const)
        return !sort_consteval(curr->bf.op, &curr->bf.right, &curr->bf.left);
    }
    return 0;
  }
  case UNARY: {
    return reorder_cleanup_imp(curr->uf.in);
  }
  case CONSTANT:
    return 1;
  case IDENTITY:
    return 0;
  }
  fprintf(stderr, "[FATAL] Reached end of control statement in %s.\n",
          __func__);
  exit(1);
#else
  return 0;
#endif
}
