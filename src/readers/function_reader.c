#include "function_reader.h"

static f_node *least_significant_token_imp(const token_array *args) { 
  size_t sp = 0;
  token split_data = args->data[sp];
  for (size_t i = 0; i < args->size; i++) {
    token *curr = &args->data[i];
    //fprintf(stdout, "Current %s \n", curr->token->key.cstring);

    if (curr->priority > split_data.priority) continue;
    if (curr->priority == split_data.priority) {
      if (curr->token->value.priority >= split_data.token->value.priority) continue;
    }
    sp = i;
    split_data = *curr;
    //fprintf(stdout, "Passed.\n");
    //if (curr->priority < split_data.priority) {
    //  sp = i;
    //  split_data = *curr;
    //  fprintf(stdout, "Paren priority: new %s \n", curr->token->key.cstring);
    //  continue;
    //}
    //if (curr->token->value.priority < split_data.token->value.priority) {
    //  sp = i;
    //  split_data = *curr;
    //  fprintf(stdout, "Oper priority: new %s \n", curr->token->key.cstring);
    //  continue;
    //}
  }
  fprintf(stdout, "Found %s \n", split_data.token->key.cstring);

  switch (split_data.token->value.ty) {
  case UOPER: {
    f_node *in = least_significant_token_recurse(&(token_array){
        .data = args->data + sp + 1, .size = (args->size - sp - 1)});
    if (NULL == in)
      return NULL;
    f_node *uoper = new_fnode();
    *uoper =
        (f_node){.name = split_data.token->key,
                 .ty = UNARY,
                 .uf = {.op = split_data.token->value.uOperation, .in = in}};
    return uoper;
  }
  case BOPER: {
    f_node *left = least_significant_token_recurse(
        &(token_array){.data = args->data, .size = sp});
    if (NULL == left)
      return NULL;
    f_node *right = least_significant_token_recurse(&(token_array){
        .data = (args->data + sp + 1), .size = (args->size - sp - 1)});
    if (NULL == right) {
      free_fnode_recurse(left);
      return NULL;
    }
    f_node *boper = new_fnode();
    *boper = (f_node){.name = split_data.token->key,
                      .ty = BINARY,
                      .bf = {.op = split_data.token->value.bOperation,
                             .left = left,
                             .right = right},
                      .priority = split_data.priority};
    return boper;
  }
  case FUNC:
    return split_data.token->value.fObject.root;
  case VECTOR: {
    f_node *constant = new_fnode();
    gString name = g_from_capacity(10);
    sprint_vector(&name, &split_data.token->value.vLiteral);
    *constant =
        (f_node){.ty = CONSTANT,
                 .name = {.cstring = name.cstring, .size=name.size},
                 .output = copy_vdliteral(&split_data.token->value.vLiteral),
                 .priority = split_data.priority};
    return constant;
  }
  case TEMP: {
    f_node *identity = new_fnode();
    gString name = g_from_capacity(7*split_data.token->value.vLiteral.size);
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
    
    *identity =
        (f_node){.ty = IDENTITY, .name = {.cstring=name.cstring, .size=name.size},
          .xf.index = split_data.token->value.other, .priority = split_data.priority};
    return identity;
  }
  default: {
    fprintf(stderr, "[FATAL] Invalid token %s passed to %s.\n",
            split_data.token->key.cstring, __func__);
    exit(1);
  }
  } 
}

__attribute__((always_inline)) inline f_node *least_significant_token_recurse(const token_array *args) {
  if (args->size <= 0) {
    fprintf(stderr,
            "[ERROR] Recursive call in %s failed to reach a base case.\n",
            __func__);
    return NULL;
  }
  return least_significant_token_imp(args);
}
