#include "vector.h"

Object as_vdliteral(vd_literal *value) {
  return (Object){.vLiteral = *value, .ty = VECTOR, .priority = PRIORITY_MAX};
}

Object as_vdliteral_mv(vd_literal value) {
  return (Object){.vLiteral = value, .ty = VECTOR, .priority = PRIORITY_MAX};
}

vd_literal make_scalar(double value) {
  vd_literal s = alloc_vdliteral(SCALAR);
  *s.data = value;
  return s;
}

void free_vdliteral(vd_literal *lit) {
  if (NULL != lit->data) {
    free(lit->data);
  }
  lit->size = 0;
}

__attribute__((always_inline)) inline vd_literal copy_vdliteral(const vd_literal* other) {
  vd_literal lit = alloc_vdliteral(other->size);
  if (SCALAR == lit.size) {
    *(lit.data) = *other->data;
    lit.size = other->size; 
    return lit;
  }
  for (vector_size_t i = 0; i < lit.size; i++) {
    lit.data[i] = other->data[i];
  }
  lit.size = other->size; 
  return lit;
}

__attribute__((always_inline)) inline vd_literal alloc_vdliteral(vector_size_t n) {
  vector_size_t size = (SCALAR == n) * 1 + (SCALAR != n) * n;
  vd_literal lit =
      (vd_literal){.data = (double *)calloc(size, sizeof(double)), .size = n};
  if (NULL == lit.data) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return lit;
}

__attribute__((always_inline)) inline void sprint_vector(gString *inp, const vd_literal *value) {
  if (SCALAR == value->size) {
    const size_t buff_size = 17;
    char buff[buff_size];
    int len = snprintf(buff, buff_size - 1, "%lf", value->data[0]);
    if (0 > len) {
      fprintf(stderr, "Unknown error occurred in snprintf for %s.\n", __func__);
      exit(1);
    }
    g_append_back(inp, buff, len);
    return;
  }
  g_append_back_c(inp, "[");
  for (vector_size_t i = 0; i < value->size; i++) {
    const size_t buff_size = 17;
    char buff[buff_size];
    int len = snprintf(buff, buff_size - 1, "%lf,", value->data[i]);
    if (0 > len) {
      fprintf(stderr, "Unknown error occurred in snprintf for %s.\n", __func__);
      exit(1);
    }
    g_append_back(inp, buff, len);
  }
  g_append_back_c(inp, "]");
}


