#include "simplify.h"

/*f_object simplify_imp(f_object *fun) {
  vector_list out_cache = alloc_vdlist(fun->attr.depth, fun->attr.out_size);
  simplification_t simp;
  f_node *root = fnode_copysimplify_recurse(fun->root, &simp, out_cache.data);

  if (NULL == root) {
    gString name = g_from_capacity(10);
    sprint_vector(&name, &out_cache.data[0]);
    root = new_fnode();
    *root = (f_node){.ty = CONSTANT,
                      .cf.output =
                          copy_vdliteral(&out_cache.data[0]),
                      .depth_index = 0,
                      .priority = 0,
                      .name = {.cstring = name.cstring, .size = name.size}};
  }

  free_vdlist(&out_cache);

  return (f_object){.root = root, .attr = fun->attr};
}*/

/*
typedef enum Simplification {
  NOTHING,
  CONSTEVAL,
  REMOVE,
} simplification_t;

simplification_t resolve_simplification(char fname,
                                        const vd_literal *consteval) {
  switch (fname) {
  case '+':
    if (is_num(consteval, 0, 0.00000000001)) {
      return REMOVE;
    }
    return NOTHING;
  case '-':
    return NOTHING;
  case '*':
    if (is_num(consteval, 0, 0.00000000001)) {
      return CONSTEVAL;
    }
    if (is_num(consteval, 1, 0.00000000001)) {
      return REMOVE;
    }
    return NOTHING;
  case '/': {
    return NOTHING;
  }
  case '^':
    if (is_num(consteval, 0, 0.00000000001)) {
      return CONSTEVAL;
    }
    if (is_num(consteval, 1, 0.00000000001)) {
      return REMOVE;
    }
    return NOTHING;
  case '_':
    return NOTHING;
  default: {
    fprintf(stderr, "Unknown name passed to %s: %c", __func__, fname);
    exit(5);
  }
  }
}

void simplify_command(Object *obj, token_array *args) {
  if (1 != args->size) {
    fprintf(stderr, "[ERROR] Please provide one function to simplify.\n");
    *obj = null_object();
    return;
  }
  Object *inp = &args->data[0].token->value;
  f_object *inp_func;
  switch (inp->ty) {
  case PFUNC:
    inp_func = inp->pObject.fObj;
    break;
  case FUNC:
    inp_func = &inp->fObject;
    break;
  default:
    fprintf(stderr, "[ERROR] %s is not a function.\n",
            args->data[0].token->key.cstring);
    *obj = null_object();
    return;
  }

  f_object out = simplify_imp(inp_func);
  *obj = (Object){.ty = FUNC, .fObject = out};
  return;
}

static f_node *fnode_copysimplify_recurse(f_node *func, simplification_t *simp,
                                          vd_literal *write_buff) {
  f_node *out = NULL;
  switch (func->ty) {
  case BINARY: {
    simplification_t rsimp;
    f_node *left = fnode_copysimplify_recurse(func->bf.left, simp, write_buff);
    f_node *right =
        fnode_copysimplify_recurse(func->bf.right, &rsimp, write_buff);
    switch (*simp) {
    case CONSTEVAL: {
      switch (rsimp) {
      case CONSTEVAL: // both consteval? return consteval
        func->bf.op(&write_buff[func->depth_index],
                    &write_buff[func->depth_index],
                    &write_buff[func->depth_index + 1]);
        return NULL;
      case NOTHING: // right not consteval?
        switch (*simp = resolve_simplification(func->name.cstring[0],
                                       &write_buff[func->depth_index])) {
        // will fail if so ever a binary function is introduced that is not one
        // char
        case NOTHING: {
          fprintf(stderr, "NOTHING R:\n");
          left = new_fnode();
          gString name = g_from_capacity(10);
          sprint_vector(&name, &write_buff[func->depth_index]);
          *left = (f_node){
              .ty = CONSTANT,
              .cf.output = copy_vdliteral(&write_buff[func->depth_index]),
              .depth_index = func->depth_index,
              .priority = func->priority,
              .name = {.cstring = name.cstring, .size = name.size}};
          out = new_fnode();
          out->bf.left = left;
          out->bf.right = right;
          fprintf(stderr, "NOTHING R:\n");
          *simp = NOTHING;
          }
          break;
        case CONSTEVAL: {
          fprintf(stderr, "CONSTEVAL:\n");
          func->bf.op(&write_buff[func->depth_index],
                      &write_buff[func->depth_index],
                      &write_buff[func->depth_index + 1]);
          free_fnode_recurse(right);
          *simp = CONSTEVAL;
          return NULL;
          }
        case REMOVE: {
          fprintf(stderr, "REMOVE:\n");
          *simp = NOTHING;
          return right; // left has no effect
        }
        }
        break;
      case REMOVE:
      exit(5);
      }
    }
    break;
    case NOTHING: {
      switch (rsimp) {
      case CONSTEVAL: // left not consteval?
        switch (*simp = resolve_simplification(func->name.cstring[0],
                                       &write_buff[func->depth_index + 1])) {
        case NOTHING: {
          fprintf(stderr, "NOTHING L:\n");
          right = new_fnode();
          gString name = g_from_capacity(10);
          sprint_vector(&name, &write_buff[func->depth_index + 1]);
          *right = (f_node){
              .ty = CONSTANT,
              .cf.output = copy_vdliteral(&write_buff[func->depth_index + 1]),
              .depth_index = func->depth_index + 1,
              .priority = func->priority,
              .name = {.cstring = name.cstring, .size = name.size}};
          out = new_fnode();
          out->bf.right = right;
          out->bf.left = left;
          fprintf(stderr, "NOTHING L:\n");
          *simp = NOTHING;
          }
          break;
        case CONSTEVAL: {
          fprintf(stderr, "CONSTEVAL:\n");
          func->bf.op(&write_buff[func->depth_index],
                      &write_buff[func->depth_index],
                      &write_buff[func->depth_index + 1]);
          free_fnode_recurse(left);
          *simp = CONSTEVAL;
          return NULL;
          }
        case REMOVE: {
          fprintf(stderr, "REMOVE:\n");
          *simp = NOTHING;
          return left; // right has no effect
          }
      }
      break;
      case NOTHING: // neither consteval? return copy
        out = new_fnode();
        out->bf.op = func->bf.op;
        out->bf.left = left;
        out->bf.right = right;
        break;
      case REMOVE:
        exit(5);
      }
    }
    break;
    case REMOVE:
      exit(5);
    }
  }
  break;
  case UNARY: {
    f_node *in = fnode_copysimplify_recurse(func->uf.in, simp, write_buff);
    switch (*simp) {
    case CONSTEVAL:
      func->uf.op(&write_buff[func->depth_index],
                  &write_buff[func->depth_index]);
      return NULL;
    case NOTHING:
      out = new_fnode();
      out->uf.op = func->uf.op;
      out->uf.in = in;
      break;
    case REMOVE:
      exit(5);
    }
  }
  break;
  case CONSTANT: {
    *simp = CONSTEVAL;
    vu_set(&write_buff[func->depth_index],
           &func->cf.output); // copy value to buffer
    return NULL;
  }
  case IDENTITY: {
    *simp = NOTHING;
    out = new_fnode(); // copy full node
    out->xf.index = func->xf.index;
    break;
  }
  }
  fprintf(stderr, "Copy\n");
  out->ty = func->ty;
  out->priority = func->priority;
  out->depth_index = func->depth_index;
  out->name = m_from_copy(func->name);
  return out;
}

*/
