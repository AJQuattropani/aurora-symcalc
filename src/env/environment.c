#include "environment.h"

void default_map(Map *map) {
  cinsert(map, "exit", (_value){.mContext = exit_env, .ty = CONTEXT});
  cinsert(map, "reset", (_value){.mContext = reset_env, .ty = CONTEXT});
  cinsert(map, "open", (_value){.mContext = open_file, .ty = CONTEXT});
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
