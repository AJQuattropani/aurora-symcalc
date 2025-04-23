#include "function_reader.h"

static f_node *least_significant_token_recurse(const token_array *args, f_object *head);

static f_node *least_significant_token_imp(const token_array *args, f_object *head) { 
  size_t sp = 0;
  token split_data = args->data[sp];
  for (size_t i = 0; i < args->size; i++) {
    token *curr = &args->data[i];
    if (curr->priority > split_data.priority) continue;
    if (curr->priority == split_data.priority) {
      if (curr->token->value.priority >= split_data.token->value.priority) continue;
    }
    sp = i;
    split_data = *curr;
  }
  fprintf(stdout, "Found %s \n", split_data.token->key.cstring);

  switch (split_data.token->value.ty) {
  case UOPER: {
    f_node *in = least_significant_token_recurse(&(token_array){
        .data = args->data + sp + 1, .size = (args->size - sp - 1)}, head);
    if (NULL == in)
      return NULL;
    in->out = &head->cache.uf.in;
    f_node *uoper = new_fnode();
    *uoper =
        (f_node){.name = split_data.token->key,
                 .ty = UNARY,
                 .uf = {.op = split_data.token->value.uOperation, .in = in},
                 .priority = split_data.priority,
                 .out = &head->cache.out};
    return uoper;
  }
  case BOPER: {
    f_node *left = least_significant_token_recurse(
        &(token_array){.data = args->data, .size = sp}, head);
    if (NULL == left)
      return NULL;
    f_node *right = least_significant_token_recurse(&(token_array){
        .data = (args->data + sp + 1), .size = (args->size - sp - 1)}, head);
    if (NULL == right) {
      free_fnode_recurse(left);
      return NULL;
    }
    left->out = &head->cache.bf.left;
    right->out = &head->cache.bf.right;
    f_node *boper = new_fnode();
    *boper = (f_node){.name = split_data.token->key,
                      .ty = BINARY,
                      .bf = {.op = split_data.token->value.bOperation,
                             .left = left,
                             .right = right},
                      .priority = split_data.priority,
                      .out = &head->cache.out};
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
                 .cf = {.output = copy_vdliteral(&split_data.token->value.vLiteral)},
                 .priority = split_data.priority,
                 .out = &head->cache.out}; // in case function is just vector point to out
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
          .xf.index = split_data.token->value.other, .priority = split_data.priority,
          .out = &head->cache.out};
    return identity;
  }
  default: 
  {
    fprintf(stderr, "[FATAL] Invalid token %s passed to %s.\n",
            split_data.token->key.cstring, __func__);
    exit(1);
  }
  } 
}

__attribute__((always_inline)) inline f_node *least_significant_token_recurse(const token_array *args, f_object *head) {
  if (args->size <= 0) {
    fprintf(stderr,
            "[ERROR] Recursive call in %s failed to reach a base case.\n",
            __func__);
    return NULL;
  }
  return least_significant_token_imp(args, head);
}

void read_function(Object* obj, const token_array *args) {
  argcnt_t argnum = 0;
  for (; argnum < args->size; argnum++) {
    if (SYNTAX_RIGHT == args->data[argnum].token->value.ty)
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

  // prepass to avoid an obvious source of recursive deletion hell
  for (size_t i = offs; i < args->size; i++) {
    if (args->data[i].token->value.ty == UOPER ||
        args->data[i].token->value.ty == BOPER ||
        args->data[i].token->value.ty == TEMP ||
        args->data[i].token->value.ty == FUNC ||
        args->data[i].token->value.ty == VECTOR ||
        args->data[i].token->value.ty == NONE)
      continue;
    fprintf(stderr, "[ERROR] Invalid token %s passed to %s.\n",
            args->data[i].token->key.cstring, __func__);
    *obj = null_object();
    return;
  }
  
  *obj = (Object){.fObject = {.root = NULL, .argcnt = argnum, 
    .cache.bf = {}}, .ty = FUNC, .priority = PRIORITY_MAX};
  
  f_node *root = least_significant_token_recurse(
      &(token_array){.data = args->data + offs, .size = args->size - offs}, &obj->fObject);
  if (NULL == root) {
    *obj = null_object();
    return;
  }

  obj->fObject.root = root;
  obj->fObject.cache.bf.left = alloc_vdliteral(4); 
  obj->fObject.cache.bf.right = alloc_vdliteral(4);
}


