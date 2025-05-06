#include "function_reader.h"

static f_node *least_significant_token_recurse(token_array *args, depth_t depth,
                                               f_attribs *attr);

static f_node *least_significant_token_imp(token_array *args, depth_t depth,
                                           f_attribs *attr) {
  update_depth_max(depth, &attr->depth);

  size_t sp = 0;
  token split_data = args->data[sp];
  for (size_t i = 0; i < args->size; i++) {
    token *curr = &args->data[i];
    if (curr->priority > split_data.priority)
      continue;
    if (curr->priority == split_data.priority) {
      if (curr->token->value.priority >= split_data.token->value.priority)
        continue;
    }
    sp = i;
    split_data = *curr;
  }
  // fprintf(stdout, "Found %s \n", split_data.token->key.cstring);

  switch (split_data.token->value.ty) {
  case BOPER: {
    f_node *left = least_significant_token_recurse(
        &(token_array){.data = args->data, .size = sp}, depth, attr);
    if (NULL == left)
      return NULL;
    f_node *right = least_significant_token_recurse(
        &(token_array){.data = (args->data + sp + 1),
                       .size = (args->size - sp - 1)},
        depth + 1, attr);
    if (NULL == right) {
      free_fnode_recurse(left);
      return NULL;
    }
    f_node *boper = new_fnode();
    *boper = (f_node){.name = m_from_copy(split_data.token->key),
                      .ty = BINARY,
                      .bf = {.op = split_data.token->value.bOperation,
                             .left = left,
                             .right = right},
                      .priority = split_data.priority,
                      .depth_index = depth};
    args->data[sp].token = NULL;
    return boper;
  }
  case UOPER: {
    f_node *in = least_significant_token_recurse(
        &(token_array){.data = args->data + sp + 1,
                       .size = (args->size - sp - 1)},
        depth, attr);
    if (NULL == in)
      return NULL;
    f_node *uoper = new_fnode();
    *uoper =
        (f_node){.name = m_from_copy(split_data.token->key),
                 .ty = UNARY,
                 .uf = {.op = split_data.token->value.uOperation, .in = in},
                 .priority = split_data.priority,
                 .depth_index = depth};
    args->data[sp].token = NULL;
    return uoper;
  }
  case VECTOR: {
    f_node *constant = new_fnode();
    gString name = g_from_capacity(10);
    sprint_vector(&name, &split_data.token->value.vLiteral);

    *constant = (f_node){
        .ty = CONSTANT,
        .name = {.cstring = name.cstring, .size = name.size},
        .cf = {.output = copy_vdliteral(&split_data.token->value.vLiteral)},
        .priority = split_data.priority,
        .depth_index = depth}; // in case function is just vector point to out
    args->data[sp].token = NULL;
    return constant;
  }
  case TEMP: {
    f_node *identity = new_fnode();
    gString name = g_from_capacity(7 * split_data.token->value.vLiteral.size);
    g_append_back_c(&name, "x");
    const int b_size = 7;
    char numbuff[b_size];
    int n;
    n = snprintf(numbuff, b_size, "%d", (short)split_data.token->value.other);
    if (n >= b_size || 0 >= n) {
      fprintf(stderr, "snprintf failed in %s", __func__);
      exit(1);
    }
    g_append_back(&name, numbuff, n);

    *identity = (f_node){.ty = IDENTITY,
                         .name = {.cstring = name.cstring, .size = name.size},
                         .xf.index = split_data.token->value.other,
                         .priority = split_data.priority,
                         .depth_index = depth};
    args->data[sp].token = NULL;
    return identity;
  }
  default: {
    fprintf(stderr, "[FATAL] Invalid token %s passed to %s.\n",
            split_data.token->key.cstring, __func__);
    exit(1);
  }
  }
}

__attribute__((always_inline)) inline f_node *
least_significant_token_recurse(token_array *args, depth_t depth,
                                f_attribs *attr) {
  if (args->size <= 0) {
    fprintf(stderr,
            "[ERROR] Recursive call in %s failed to reach a base case.\n",
            __func__);
    return NULL;
  }
  return least_significant_token_imp(args, depth, attr);
}

void read_function(Object *obj, token_array *args) {
  argcnt_t argnum = 0;

  for (; argnum < args->size; argnum++) {
    if (SYNTAX_EQUALS == args->data[argnum].token->value.ty)
      break;
    if (NONE != args->data[argnum].token->value.ty) {
      fprintf(stderr, "[ERROR] Function argument collision at %s\n",
              args->data[argnum].token->key.cstring);
      *obj = null_object();
      return;
    }
    args->data[argnum].token->value = (Object){
        .ty = TEMP, .other = (uint64_t)argnum, .priority = PRIORITY_MAX};
  }

  argcnt_t offs = argnum + 1;
  if (offs >= args->size) {
    fprintf(stderr, "[ERROR] Please provide a function body.\n");
    *obj = null_object();
    return;
  }

  vector_size_t out_size = SCALAR;
  // prepass to avoid an obvious source of recursive deletion hell
  for (size_t i = offs; i < args->size; i++) {
    if (obj !=
        &args->data[i]
             .token->value) { // patch for preventing function self-referencing
      obj_t ty = args->data[i].token->value.ty;
      switch (ty) {
      case NONE:
        break;
      case READER:
        break;
      case SYNTAX_EQUALS:
        break;
      case TEMP:
        continue;
      case FUNC:
        continue;
      case PFUNC:
        continue;
      case BOPER:
        continue;
      case CONTEXT:
        continue;
      case UOPER:
        continue;
      case VECTOR: {
        vector_size_t size = args->data[i].token->value.vLiteral.size;
        if (out_size == size)
          continue;
        if (SCALAR == out_size) {
          out_size = size;
          continue;
        }
      }
      }
    }

    fprintf(stderr, "[ERROR] Invalid token \"%s\" passed to %s.\n",
            args->data[i].token->key.cstring, __func__);
    *obj = null_object();
    return;
  }

  *obj = (Object){
      .fObject = {.root = NULL,
                  .attr = {.argcnt = argnum, .depth = 0, .out_size = out_size}},
      .ty = FUNC,
      .priority = PRIORITY_MAX};

  f_node *root = least_significant_token_recurse(
      &(token_array){.data = args->data + offs, .size = args->size - offs}, 0,
      &obj->fObject.attr);
  if (NULL == root) {
    *obj = null_object();
    return;
  }
  for (size_t i = offs; i < args->size; i++) {
    if (args->data[i].token != NULL) {
      fprintf(stderr, "[ERROR] Function parse did not consume all tokens.\n");
      free_fnode_recurse(root);
      *obj = null_object();
      return;
    }
  }

  obj->fObject.root = root;
}

void read_copy_packed(Object *obj, token_array *args) {
  if (1 != args->size) {
    fprintf(stderr, "[ERROR] Please provide only one function to pack.\n");
    *obj = null_object();
    return;
  }
  if (FUNC != args->data[0].token->value.ty) {
    *obj = null_object();
    return;
  }
  Object *function = &args->data[0].token->value;
  *obj = (Object){.pObject = make_packed_copy(&function->fObject),
                  .ty = PFUNC,
                  .priority = PRIORITY_MAX};
}
