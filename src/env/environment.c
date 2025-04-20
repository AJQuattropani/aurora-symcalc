#include "environment.h"

void default_map(Map map) {
  cinsert(map, "exit", (_value){.mContext = exit_env, .ty = CONTEXT});
  cinsert(map, "reset", (_value){.mContext = reset_env, .ty = CONTEXT});
  cinsert(map, "printenv", (_value){.mContext = print_env, .ty = CONTEXT});
  cinsert(map, "peak", (_value){.mContext = print_tok, .ty = CONTEXT});
  cinsert(map, "set", (_value){.mContext = define_object, .ty = CONTEXT});
  cinsert(map, "open", (_value){.mContext = open_files, .ty = CONTEXT});
  cinsert(map, "return", (_value){.mContext = return_env, .ty = CONTEXT});
  cinsert(map, "+", (_value){.bOperation = vb_add, .ty = BOPER});
  cinsert(map, "-", (_value){.bOperation = vb_sub, .ty = BOPER});
  cinsert(map, "*", (_value){.bOperation = vb_mul, .ty = BOPER});
  cinsert(map, "/", (_value){.bOperation = vb_div, .ty = BOPER});
  cinsert(map, "^", (_value){.bOperation = vb_pow, .ty = BOPER});
  cinsert(map, "_", (_value){.bOperation = vb_log, .ty = BOPER});
  cinsert(map, "\\sin", (_value){.uOperation = vu_sin, .ty = UOPER});
  cinsert(map, "\\cos", (_value){.uOperation = vu_cos, .ty = UOPER});
  cinsert(map, "\\tan", (_value){.uOperation = vu_tan, .ty = UOPER});
  cinsert(map, "\\sec", (_value){.uOperation = vu_sec, .ty = UOPER});
  cinsert(map, "\\csc", (_value){.uOperation = vu_csc, .ty = UOPER});
  cinsert(map, "\\cot", (_value){.uOperation = vu_cot, .ty = UOPER});
  cinsert(map, "\\log", (_value){.uOperation = vu_log, .ty = UOPER});
  cinsert(map, "\\arcsin", (_value){.uOperation = vu_asin, .ty = UOPER});
  cinsert(map, "\\arccos", (_value){.uOperation = vu_acos, .ty = UOPER});
  cinsert(map, "\\arctan", (_value){.uOperation = vu_atan, .ty = UOPER});
  cinsert(map, "VECTOR", (_value){.reader = read_vector, .ty = READER});
}

static inline void init_stack(sc_stack *stack, int argc, char *argv[]) {
  push_file(stack, stdin);
  for (size_t i = argc - 1; i > 0; i--) {
    if (0 != open_file(stack, argv[i])) {
      fprintf(stderr, "Failed to open file %s.\n", argv[i]);
      exit(1);
    }
  }
}

void init_env(env *env, int argc, char *argv[]) {
  init_stack(&env->script_stack, argc, argv);
  default_map(env->map);
  env->status = OK;
  env->output_buffer = g_from_capacity(256);
}

void free_env(env *env) {
  destroy_stack(&env->script_stack);
  empty_map(env->map);
  g_deletestr(&env->output_buffer);
}

#define OUTPUT_BUFFER_SIZE(x) x * 3

void runtime(env *env) {
  mString mstr;
  vList vlist = v_from_capacity(10);
  FILE *current_file;
  while (NULL != (current_file = get_current_file(&env->script_stack))) {
    while (0 <= v_get_line(&vlist, &mstr, current_file)) {
      if (stdin != current_file) {
        g_append_back_c(&env->output_buffer, mstr.cstring);
        g_append_back_c(&env->output_buffer, " \n");
      }

      token_array arr = tokenize(env->map, &vlist);

      if (NULL == arr.data || 0 >= arr.size) {
        destroy_token_array(&arr);
        update_map(env->map);
        g_empty(&env->output_buffer);
        continue;
      }

      Object *obj = &arr.data[0].token->value;
      switch (obj->ty) {
      case CONTEXT: {
        if (CONTEXT == obj->ty) {
          mf_context mc = obj->mContext;
          mc(env, &arr);
        }
      } break;
      case VECTOR: {
        sprint_object(&env->output_buffer, obj);
      } break;
      case NONE: {
        g_append_back_c(&env->output_buffer, "Unknown token.");
      }
      default:
        break;
      }

      printf("%s\n", env->output_buffer.cstring);
      
      destroy_token_array(&arr);
      update_map(env->map);
      g_empty(&env->output_buffer);

      if (OK != env->status) {
        break;
      }
    }
    
    if (RETURN == env->status) {
      env->status = OK;
    }
    if (CALL == env->status) {
      env->status = OK;
      continue;
    }
    if (EXIT == env->status) {
      break;
    }

    pop_file(&env->script_stack);
  }
  v_free(&vlist);
  m_deletestr(&mstr);
}
