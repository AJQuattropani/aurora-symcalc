#include "functional.h"

void read_eval(Object *obj, token_array *args) {
  Object *func_obj = &args->data[0].token->value;
  f_object *fun;
  switch (func_obj->ty) {
  case PFUNC:
    fun = args->data[0].token->value.pObject.fObj;
    break;
  case FUNC:
    fun = &args->data[0].token->value.fObject;
    break;
  default:
    fprintf(stderr, "[ERROR] Unexpected input in %s\n", __func__);
    *obj = null_object();
    return;
  }

  Object out = function_eval(fun, args);
  switch (out.ty) {
  case VECTOR:
    *obj = out;
    return;
  case FUNC:
    *obj = (Object){.ty = FUNC, .fObject = copy_fobject(fun)};
    return;
  default:
    fprintf(stderr, "Unhandled type passed to %s.\n", __func__);
    exit(3);
  }
}

Object function_eval(f_object *fun, const token_array *args) {
  if (1 == args->size) {
    return (Object){.ty = FUNC, 0};
  }

  Object output = null_object();
  vector_size_t inp_size = fun->attr.out_size;
  if (fun->attr.argcnt == args->size - 1) {
    vd_literal *inp_args =
        (vd_literal *)malloc(sizeof(vd_literal) * fun->attr.argcnt);
    for (size_t i = 0; i < fun->attr.argcnt; i++) {
      Object *o = &args->data[i + 1].token->value;
      if (0 > args->data[i].priority) {
        fprintf(stderr, "[ERROR] Arguments are poorly formatted.\n");
        goto cleanup;
      }
      switch (o->ty) {
      case VECTOR:
        if (inp_size != o->vLiteral.size && SCALAR != o->vLiteral.size &&
            SCALAR != inp_size) {
          fprintf(stderr,
                  "[ERROR] Inputs are not uniformly sized with output.\n");
          goto cleanup;
        }

        inp_args[i] = o->vLiteral;
        inp_size = o->vLiteral.size;
        break;
      case FUNC:
        // TODO IMPLEMENT FUNCTION COMPOSITION
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        goto cleanup; // todo implement function composition
      default:
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        goto cleanup;
      }
    }
    // hybrid buffer allocates all memory for vectors and literals
    vd_literal *out_cache =
        (vd_literal *)malloc(sizeof(vd_literal) * fun->attr.depth);
    if (NULL == out_cache) {
      fprintf(stderr, "[ERROR] malloc failed in %s\n", __func__);
      exit(1);
    }
    for (size_t i = 0; i < fun->attr.depth; i++) {
      out_cache[i] = alloc_vdliteral(inp_size);
    }

    evaluate_function_imp(fun->root, out_cache, inp_args);

    output = (Object){.ty = VECTOR, .vLiteral = out_cache[0]};

    for (size_t i = 1; i < fun->attr.depth; i++) {
      free_vdliteral(&out_cache[i]);
    }
    free(out_cache);

  cleanup:
    free(inp_args);
  }
  return output;
}

void function_command(env *context, f_object *fun, const token_array *args) {
  Object o = function_eval(fun, args);
  switch (o.ty) {
  case FUNC:
    sprint_function(&context->output_buffer, fun);
    break;
  case VECTOR:
    g_append_back(&context->output_buffer, args->data[0].token->key.cstring,
                  args->data[0].token->key.size);
    g_append_back_c(&context->output_buffer, " = ");
    sprint_vector(&context->output_buffer, &o.vLiteral);
    free_vdliteral(&o.vLiteral);
    break;
  default:
    fprintf(stderr, "[ERROR] Unhandled type passed to %s.", __func__);
    exit(3);
  }
}

void evaluate_function_imp(f_node *fun, vd_literal *out, const vd_literal *in) {
  switch (fun->ty) {
  case BINARY:
    evaluate_function_imp(fun->bf.left, out, in);
    evaluate_function_imp(fun->bf.right, out, in);
    fun->bf.op(&out[fun->depth_index], &out[fun->depth_index],
               &out[fun->depth_index + 1]);
    return;
  case UNARY:
    evaluate_function_imp(fun->uf.in, out, in);
    fun->uf.op(&out[fun->depth_index], &out[fun->depth_index]);
    return;
  case IDENTITY: // optimize this.
    vu_set(&out[fun->depth_index], &in[fun->xf.index]);
    return;
  case CONSTANT:
    vu_set(&out[fun->depth_index], &fun->cf.output);
    return;
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

typedef enum Simplification {
  NOTHING,
  CONSTEVAL,
  REMOVE,
} simplification_t;

simplification_t resolve_simplification(char fname,
                                        const vd_literal *consteval) {
  // TODO REPLACE THIS WITH A RESOLVE THAT RETURNS A NODE, SO THAT ORDERED
  // OPERATIONS LIKE
  // - and / MAY RECIEVE SIMPLIFICATIONS
  // more aggressive forms of simplification may still be implemented
  // if an argument is not used, remove it.
  // if multiplication by 0, remove it.
  // evaluate constant branches.
  // if addition or subtraction by 0, remove 0
  // if multiplication by 1, remove 1.
  // if exponentiation by 0, set 1.
  // if exponentiation by 1, remove it.
  switch (fname) {
  case '+':
    if (is_num(consteval, 0, 0.00000000001)) {
      return REMOVE;
    }
    return NOTHING;
  case '-':
    return NOTHING;
  case '*':
    if (is_num(consteval, 0, 0.0000000001)) {
      return CONSTEVAL;
    }
    if (is_num(consteval, 1, 0.0000000001)) {
      return REMOVE;
    }
    return NOTHING;
  case '/': {
    return NOTHING;
  }
  case '^':
    if (is_num(consteval, 0, 0.0000000001)) {
      return CONSTEVAL;
    }
    if (is_num(consteval, 1, 0.0000000001)) {
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

static f_node *fnode_copysimplify_recurse(f_node *func, simplification_t *simp,
                                          vd_literal *write_buff) {
  //
  // to get desired behavior, push up consteval in any situation where consteval
  // is warranted. when consteval is reached in a binary function, or at the
  // root, call a function evaluation on the consteval branch with any empty
  // vector (output is independent of input) and set the node to a constant when
  // an identity is found, it should increment the counter. if identity is
  // consteval'd on, it should decrement.
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
        switch (resolve_simplification(func->name.cstring[0],
                                       &write_buff[func->depth_index])) {
        // will fail if so ever a binary function is introduced that is not one
        // char
        case NOTHING:
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
          break;
        case CONSTEVAL:
          func->bf.op(&write_buff[func->depth_index],
                      &write_buff[func->depth_index],
                      &write_buff[func->depth_index + 1]);
          free_fnode_recurse(right);
          return NULL;
        case REMOVE:
          *simp = NOTHING;
          return right; // left has no effect
        }
        break;
      case REMOVE:
        exit(5);
      }
      break;
    }
    case NOTHING: {
      switch (rsimp) {
      case CONSTEVAL: // left not consteval?
        switch (resolve_simplification(func->name.cstring[0],
                                       &write_buff[func->depth_index + 1])) {
        case NOTHING:
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
          break;
        case CONSTEVAL:
          fprintf(stderr, "CONSTEVAL:\n");
          func->bf.op(&write_buff[func->depth_index],
                      &write_buff[func->depth_index],
                      &write_buff[func->depth_index + 1]);
          free_fnode_recurse(left);
          return NULL;
        case REMOVE:
          *simp = NOTHING;
          return left; // right has no effect
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
  out->ty = func->ty;
  out->priority = func->priority;
  out->depth_index = func->depth_index;
  out->name = m_from_copy(func->name);
  return out;
}

f_object simplify_imp(f_object *fun) {
  vd_literal *write_buff =
      (vd_literal *)malloc(sizeof(vd_literal) * fun->attr.depth);
  if (NULL == write_buff) {
    fprintf(stderr, "[ERROR] malloc failed in %s\n", __func__);
    exit(1);
  }
  for (size_t i = 0; i < fun->attr.depth; i++) {
    write_buff[i] = alloc_vdliteral(fun->attr.out_size);
  }
  simplification_t simp;
  f_node *root = fnode_copysimplify_recurse(fun->root, &simp, write_buff);

  if (NULL == root) {
    gString name = g_from_capacity(10);
    sprint_vector(&name, &write_buff[0]);
    root = new_fnode();
    *root = (f_node){.ty = CONSTANT,
                      .cf.output =
                          copy_vdliteral(&write_buff[0]),
                      .depth_index = 0,
                      .priority = 0,
                      .name = {.cstring = name.cstring, .size = name.size}};
  }

  for (size_t i = 0; i < fun->attr.depth; i++) {
    free_vdliteral(&write_buff[i]);
  }
  free(write_buff);

  return (f_object){.root = root, .attr = fun->attr};
}
