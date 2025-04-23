#include "functional.h"

void function_command(env *context, const token_array *args) {
  f_object *fun = &args->data[0].token->value.fObject;
  if (1 == args->size) {
    sprint_function(&context->output_buffer, fun);
    return;
  }
  
 vector_size_t inp_size = args->data[1].token->value.vLiteral.size; 
  if (fun->argcnt == args->size - 1) {
    vd_literal* inp_args = (vd_literal*)malloc(sizeof(vd_literal) * fun->argcnt);
    for (size_t i = 0; i < fun->argcnt; i++) {
      Object *o = &args->data[i+1].token->value;
      //if (1 != args->data[i].priority) {
      //  fprintf(stderr, "[ERROR] Arguments are poorly formatted.\n");
      //  goto cleanup;
      //}
      switch (o->ty) {
      case VECTOR:
        if (inp_size != o->vLiteral.size) {
          fprintf(stderr, "[ERROR] Inputs are not uniformly sized.\n");
        }
        inp_args[i] = o->vLiteral;
        break;
      case FUNC:
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        goto cleanup; // todo implement function composition
      default:
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        goto cleanup;
      }
    }
    free_vdliteral(&fun->cache.bf.left);
    free_vdliteral(&fun->cache.bf.right);
    fun->cache.bf.left = alloc_vdliteral(inp_size);
    fun->cache.bf.right = alloc_vdliteral(inp_size);
    evaluate_function_imp(fun->root, inp_args); 
  cleanup:
    g_append_back(&context->output_buffer, args->data[0].token->key.cstring, args->data[0].token->key.size);
    g_append_back_c(&context->output_buffer, "=");
    sprint_vector(&context->output_buffer, &fun->cache.out);
    free(inp_args);
    return;
  }

}

void evaluate_function_imp(f_node *fun, const vd_literal *in) {
  switch(fun->ty) {
  case BINARY:
    evaluate_function_imp(fun->bf.left, in);
    evaluate_function_imp(fun->bf.right, in);
    fun->bf.op(fun->out, fun->bf.left->out, fun->bf.right->out);
    return;
  case UNARY:
    evaluate_function_imp(fun->uf.in, in);
    fun->uf.op(fun->out, fun->uf.in->out);
    return;
  case SUBFUNC:
    evaluate_function_imp(fun->fn.fun->root, in); // output of f already rooted 
    vu_set(fun->out, &fun->fn.fun->cache.out);
      // to output of function
    return;
  case IDENTITY: // optimize this.
    vu_set(fun->out, &in[fun->xf.index]);
    return;
  case CONSTANT:
    vu_set(fun->out, &fun->cf.output);
    return;
  }
}


