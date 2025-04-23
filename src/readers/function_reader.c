#include "function_reader.h"

static f_node *least_significant_token_recurse(const token_array *args);

static f_node *least_significant_token_imp(const token_array *args) { 
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
        .data = args->data + sp + 1, .size = (args->size - sp - 1)});
    if (NULL == in)
      return NULL;
    f_node *uoper = new_fnode();
    *uoper =
        (f_node){.name = split_data.token->key,
                 .ty = UNARY,
                 .uf = {.op = split_data.token->value.uOperation, .in = in},
                 .priority = split_data.priority};
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
  default: 
  {
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

Object read_function(const token_array *args) {
  unsigned short argnum = 0;
  for (; argnum < args->size; argnum++) {
    if (SYNTAX_RIGHT == args->data[argnum].token->value.ty)
      break;
    if (NONE != args->data[argnum].token->value.ty) {
      fprintf(stderr, "[ERROR] Function argument collision at %s\n",
              args->data[argnum].token->key.cstring);
      return null_object();
    }
    args->data[argnum].token->value = (Object){
        .ty = TEMP, .other = (uint64_t)argnum, .priority = PRIORITY_MAX};
  }

  unsigned short offs = argnum + 1;
  if (offs >= args->size) {
    fprintf(stderr, "[ERROR] Please provide a function body.\n");
    return null_object();
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
    return null_object();
  }

  f_node *root = least_significant_token_recurse(
      &(token_array){.data = args->data + offs, .size = args->size - offs});
  if (NULL == root) {
    return null_object();
  }

  f_object function = {.root = root, .argcnt = argnum};
  return as_fobject(&function);
}


void function_command(env *context, const token_array *args) {
  f_object *fun = &args->data[0].token->value.fObject;
  if (1 == args->size) {
    sprint_function(&context->output_buffer, fun);
    return;
  }
  
  if (fun->argcnt == args->size - 1) {
    vd_literal* inp_args = (vd_literal*)malloc(sizeof(vd_literal) * fun->argcnt);
    for (size_t i = 0; i < args->size - 1; i++) {
      Object *o = &args->data[i+1].token->value;
      if (1 != o->priority) {
        fprintf(stderr, "[ERROR] Arguments are poorly formatted.\n");
        return;
      }
      switch (o->ty) {
      case VECTOR:
        inp_args[i-1] = o->vLiteral;
        break;
      case FUNC:
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        return; // todo implement function composition
      default:
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        return;
      }
    }
    evaluate_function_imp(fun->root, inp_args); 
    free(inp_args);
  }

}

void evaluate_function_imp(f_node *fun, const vd_literal *in) {
  switch(fun->ty) {
  case BINARY:
    evaluate_function_imp(fun->bf.left, in);
    evaluate_function_imp(fun->bf.right, in);
    fun->bf.op(&fun->output, &fun->bf.left->output, &fun->bf.right->output);
    return;
  case UNARY:
    evaluate_function_imp(fun->uf.in, in);
    fun->uf.op(&fun->output, &fun->uf.in->output);
    return;
  case CONSTANT:
    return;
  case IDENTITY: // optimize this.
    free_vdliteral(&fun->output);
    fun->output = copy_vdliteral(&in[fun->xf.index]);
    return;
  }
}


