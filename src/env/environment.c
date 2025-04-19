#include "environment.h"

void default_map(Map *map) {
  cinsert(map, "exit", (_value){.mContext = exit_env, .ty = CONTEXT});
  cinsert(map, "reset", (_value){.mContext = reset_env, .ty = CONTEXT});
  //cinsert(map, "open", (_value){.mContext = open_file, .ty = CONTEXT});
  cinsert(map, "+", (_value){ .bOperation = vb_add, .ty = BOPER});
  cinsert(map, "-", (_value){ .bOperation = vb_sub, .ty = BOPER});
  cinsert(map, "*", (_value){ .bOperation = vb_mul, .ty = BOPER});
  cinsert(map, "/", (_value){ .bOperation = vb_div, .ty = BOPER});
  cinsert(map, "^", (_value){ .bOperation = vb_pow, .ty = BOPER});
  cinsert(map, "_", (_value){ .bOperation = vb_log, .ty = BOPER});
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
}

inline void init_env(env *env) {
  default_map(&env->map);
  env->current_file = stdin;
  env->status = OK;
}

inline void free_env(env *env) {
  empty_map(&env->map);
  if (stdin != env->current_file && stdout != env->current_file) {
    fclose(env->current_file);
  }
  env->current_file = NULL;
}

#define OUTPUT_BUFFER_SIZE(x) x * 3

void runtime() {
  env env;
  init_env(&env);
  {
    gString output_buffer = g_from_capacity(256);
    mString mstr;
    vList vlist = v_from_capacity(10);
    while (0 <= v_get_line(&vlist, &mstr, env.current_file)) {
      
      g_append_back(&output_buffer, "Extracted: ", 11);
      for (size_t i = 0; i < vlist.size; i++) {
        vString word = vlist.data[i];
        g_append_back(&output_buffer, word.ref, word.len);
        g_append_back(&output_buffer, " | ", 3);
      }
      printf("%s\n", output_buffer.cstring);

      token_array arr = tokenize(&env.map, &vlist);
      if (NULL == arr.data || 0 >= arr.size) continue;
      
      Object *obj = &arr.data[0].token->value;
      if (CONTEXT == obj->ty) {
        mf_context mc = obj->mContext;
        mc(&env, &arr);
      }

      destroy_token_array(&arr);
      if (EXIT == env.status) {
        break;
      }

      if (NULL == env.current_file) {
        fprintf(stderr, "Read from empty file detected, aborting.\n");
        exit(1);
      }

      g_empty(&output_buffer);
    }
    v_free(&vlist);
    m_deletestr(&mstr);
    g_deletestr(&output_buffer);
  }
  empty_map(&env.map);
}



