#include "functional.h"

void read_eval(Object *obj, const token_array *args) {
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
    case FUNC: // TODO implement generic function copy
    default:
    fprintf(stderr, "Unhandled type passed to %s.\n", __func__);
    exit(3);
  }
}

Object function_eval(f_object *fun, const token_array *args) {
  if (1 == args->size) {
    return (Object){.ty=FUNC,0};
  }
  
  Object output = null_object();
  vector_size_t inp_size = fun->attr.out_size; 
  if (fun->attr.argcnt == args->size - 1) {
    vd_literal* inp_args = (vd_literal*)malloc(sizeof(vd_literal) * fun->attr.argcnt);
    for (size_t i = 0; i < fun->attr.argcnt; i++) {
      Object *o = &args->data[i+1].token->value;
      if (0 > args->data[i].priority) {
        fprintf(stderr, "[ERROR] Arguments are poorly formatted.\n");
        goto cleanup;
      }
      switch (o->ty) {
      case VECTOR:
        if (inp_size != o->vLiteral.size 
            && SCALAR != o->vLiteral.size 
            && SCALAR != inp_size) {
          fprintf(stderr, "[ERROR] Inputs are not uniformly sized with output.\n");
          goto cleanup;
        }

        inp_args[i] = o->vLiteral;
        inp_size = o->vLiteral.size;
        break;
      case FUNC:
          //TODO IMPLEMENT FUNCTION COMPOSITION
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        goto cleanup; // todo implement function composition
      default:
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        goto cleanup;
      }
    }
    // hybrid buffer allocates all memory for vectors and literals
    vd_literal *out_cache = (vd_literal*)malloc(sizeof(vd_literal) * fun->attr.depth);
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
      g_append_back(&context->output_buffer, args->data[0].token->key.cstring, args->data[0].token->key.size);
      g_append_back_c(&context->output_buffer, " = ");
      sprint_vector(&context->output_buffer, &o.vLiteral);
      free_vdliteral(&o.vLiteral);
      break;
    default:
    fprintf(stderr, "[ERROR] Unhandled type passed to %s.", __func__);
    exit(3);
  }
}

/*void function_command(env *context, f_object *fun, const token_array *args) {
  if (1 == args->size) {
    sprint_function(&context->output_buffer, fun);
    return;
  }
  
 //vector_size_t inp_size = args->data[1].token->value.vLiteral.size; 
 vector_size_t inp_size = fun->attr.out_size; 
  if (fun->attr.argcnt == args->size - 1) {
    vd_literal* inp_args = (vd_literal*)malloc(sizeof(vd_literal) * fun->attr.argcnt);
    for (size_t i = 0; i < fun->attr.argcnt; i++) {
      Object *o = &args->data[i+1].token->value;
      if (0 > args->data[i].priority) {
        fprintf(stderr, "[ERROR] Arguments are poorly formatted.\n");
        goto cleanup;
      }
      switch (o->ty) {
      case VECTOR:
        if (inp_size != o->vLiteral.size 
            && SCALAR != o->vLiteral.size 
            && SCALAR != inp_size) {
          fprintf(stderr, "[ERROR] Inputs are not uniformly sized with output.\n");
          goto cleanup;
        }

        inp_args[i] = o->vLiteral;
        inp_size = o->vLiteral.size;
        break;
      case FUNC:
          //TODO IMPLEMENT FUNCTION COMPOSITION
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        goto cleanup; // todo implement function composition
      default:
        fprintf(stderr, "[ERROR] Invalid argument provided.\n");
        goto cleanup;
      }
    }
    // hybrid buffer allocates all memory for vectors and literals
    vd_literal *out = (vd_literal*)malloc(sizeof(vd_literal) * fun->attr.depth);
    if (NULL == out) {
      fprintf(stderr, "[ERROR] malloc failed in %s", __func__);
      exit(1);
    }
    for (size_t i = 0; i < fun->attr.depth; i++) {
      out[i] = alloc_vdliteral(inp_size);
    }

    evaluate_function_imp(fun->root, out, inp_args); 

    g_append_back(&context->output_buffer, args->data[0].token->key.cstring, args->data[0].token->key.size);
    g_append_back_c(&context->output_buffer, " = ");
    sprint_vector(&context->output_buffer, &out[0]);

    for (size_t i = 0; i < fun->attr.depth; i++) {
      free_vdliteral(&out[i]);
    }
    free(out);

  cleanup:
    free(inp_args);
    return;
  }
}*/


void evaluate_function_imp(f_node *fun, vd_literal* out, const vd_literal *in) {
  switch(fun->ty) {
  case BINARY:
    evaluate_function_imp(fun->bf.left, out, in);
    evaluate_function_imp(fun->bf.right, out, in);
    fun->bf.op(&out[fun->depth_index], &out[fun->depth_index], &out[fun->depth_index+1]);
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


void simplify_command(env *context, const token_array *args) {


}


void simplify_imp(f_node *fun) {


}


