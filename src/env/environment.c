#include "environment.h"

__attribute__((always_inline)) inline void default_map(Map map) {
  cinsert(map, "exit",
          (_value){.mContext = exit_env, .ty = CONTEXT, .priority = 0});
  cinsert(map, "reset",
          (_value){.mContext = reset_env, .ty = CONTEXT, .priority = 0});
  cinsert(map, "delete",
          (_value){.mContext = delete_object, .ty = CONTEXT, .priority = 0});
  cinsert(map, "printenv",
          (_value){.mContext = print_env, .ty = CONTEXT, .priority = 0});
  cinsert(map, "peak",
          (_value){.mContext = print_tok, .ty = CONTEXT, .priority = 0});
  cinsert(map, "set",
          (_value){.mContext = define_object, .ty = CONTEXT, .priority = 0});
  cinsert(map, "run",
          (_value){.mContext = open_files, .ty = CONTEXT, .priority = 0});
  cinsert(map, "return",
          (_value){.mContext = return_env, .ty = CONTEXT, .priority = 0});
  cinsert(map, "tree", (_value){.mContext = print_tree, .ty = CONTEXT, .priority = 0});
  cinsert(map, "=",
          (_value){.reader = read_eval, .ty = SYNTAX_EQUALS, .priority = 0});
  cinsert(map, "+", (_value){.bOperation = vb_add, .ty = BOPER, .priority = 1});
  cinsert(map, "-", (_value){.bOperation = vb_sub, .ty = BOPER, .priority = 1});
  cinsert(map, "*", (_value){.bOperation = vb_mul, .ty = BOPER, .priority = 2});
  cinsert(map, "/", (_value){.bOperation = vb_div, .ty = BOPER, .priority = 2});
  cinsert(map, "^", (_value){.bOperation = vb_pow, .ty = BOPER, .priority = 3});
  cinsert(map, "_", (_value){.bOperation = vb_log, .ty = BOPER, .priority = 3});
  cinsert(map, "--",
          (_value){.uOperation = vu_neg, .ty = UOPER, .priority = 3});
  cinsert(map, "sin",
          (_value){.uOperation = vu_sin, .ty = UOPER, .priority = 4});
  cinsert(map, "cos",
          (_value){.uOperation = vu_cos, .ty = UOPER, .priority = 4});
  cinsert(map, "tan",
          (_value){.uOperation = vu_tan, .ty = UOPER, .priority = 4});
  cinsert(map, "sec",
          (_value){.uOperation = vu_sec, .ty = UOPER, .priority = 4});
  cinsert(map, "csc",
          (_value){.uOperation = vu_csc, .ty = UOPER, .priority = 4});
  cinsert(map, "cot",
          (_value){.uOperation = vu_cot, .ty = UOPER, .priority = 4});
  cinsert(map, "log",
          (_value){.uOperation = vu_log, .ty = UOPER, .priority = 4});
  cinsert(map, "ln",
          (_value){.uOperation = vu_log, .ty = UOPER, .priority = 4});
  cinsert(map, "VECTOR",
          (_value){.reader = read_vector, .ty = READER, .priority = 0});
  cinsert(map, "LINSPACE",
          (_value){.reader = read_linspace, .ty = READER, .priority = 0});
  cinsert(map, "COUNT",
          (_value){.reader = read_countspace, .ty = READER, .priority = 0});
  cinsert(map, "FUNC",
          (_value){.reader = read_function, .ty = READER, .priority = 0});
  cinsert(map, "SCALAR",
          (_value){.reader = read_scalar, .ty = READER, .priority = 0});
  cinsert(map, "PACK",
          (_value){.reader = read_copy_packed, .ty = READER, .priority = 0});
  cinsert(
      map, "GRAD",
      (_value){.reader = differentiate_command, .ty = READER, .priority = 0});
  cinsert(map, "REDUCE",
          (_value){.reader = simplify_command, .ty = READER, .priority = 0});
  cinsert(map, "e", as_vdliteral_mv(make_scalar(M_E)));
  cinsert(map, "pi", as_vdliteral_mv(make_scalar(M_PI)));
}

__attribute__((always_inline)) static inline void
init_stack(sc_stack *restrict stack, int argc, char *restrict argv[]) {
  push_file(stack, stdin);
  for (size_t i = argc - 1; i > 0; i--) {
    if (0 != open_file(stack, argv[i])) {
      fprintf(stderr, "Failed to open file %s.\n", argv[i]);
      exit(1);
    }
  }
}

__attribute__((always_inline)) inline void init_env(env *restrict env, int argc,
                                                    char **restrict argv) {
  init_stack(&env->script_stack, argc, argv);
  default_map(env->map);
  env->status = OK;
  env->output_buffer = g_from_capacity(256);
}

__attribute__((always_inline)) inline void free_env(env *restrict env) {
  destroy_stack(&env->script_stack);
  empty_map(env->map);
  g_deletestr(&env->output_buffer);
}

#define OUTPUT_BUFFER_SIZE(x) x * 3

__attribute__((always_inline)) inline void runtime(env *restrict env) {
  mString mstr;
  vList vlist = v_from_capacity(10);
  FILE *current_file;
  while (NULL != (current_file = get_current_file(&env->script_stack))) {
    while (0 <= v_get_line(&vlist, &mstr, current_file)) {
      if (stdout != current_file) {
        g_append_back_c(&env->output_buffer, "\033[0;32m");
        g_append_back_c(&env->output_buffer, mstr.cstring);
        g_append_back_c(&env->output_buffer, " ");
        g_append_back_c(&env->output_buffer, "\033[0m");
      }
      // sprint_views(&env->output_buffer, &vlist);

      token_array arr = tokenize(env->map, &vlist);

      if (NULL == arr.data || 0 >= arr.size) {
        destroy_token_array(&arr);
        g_empty(&env->output_buffer);
        continue;
      }

      Object *obj = &arr.data[0].token->value;
      switch (obj->ty) {
      case CONTEXT: {
        mf_context mc = obj->mContext;
        mc(env, &arr);
        break;
      }
      case PFUNC: {
        g_append_back_c(&env->output_buffer, "\n\t");
        f_object *fun = arr.data[0].token->value.pObject.fObj;
        function_command(env, fun, &arr);
        break;
      }
      case FUNC: {
        g_append_back_c(&env->output_buffer, "\n\t");
        f_object *fun = &arr.data[0].token->value.fObject;
        function_command(env, fun, &arr);
        break;
      }
      case VECTOR: {
        g_append_back_c(&env->output_buffer, "\n\t");
        sprint_object(&env->output_buffer, obj);
        break;
      }
      case NONE: {
        g_append_back_c(&env->output_buffer, "\n\t");
        g_append_back_c(&env->output_buffer, "Unknown token.");
        break;
      }
      default: {
        break;
      }
      }

      fprintf(stdout, "> %s\n", env->output_buffer.cstring);

      destroy_token_array(&arr);
      g_empty(&env->output_buffer);
      update_map(env->map);

      if (OK != env->status) {
        break;
      }
    }

    if (RET == env->status) {
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
