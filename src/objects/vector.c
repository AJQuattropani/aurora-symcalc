#include "vector.h"

Object as_vdliteral(vd_literal *value) {
  return (Object){.vLiteral = *value, .ty = VECTOR, .priority = SHRT_MAX};
}

void free_vdliteral(vd_literal *lit) {
  if (NULL != lit->data) {
    free(lit->data);
  }
  lit->size = 0;
}

__attribute__((always_inline)) inline vd_literal copy_vdliteral(const vd_literal* other) {
  vd_literal lit = alloc_vdliteral(other->size);
  for (size_t i = 0; i < lit.size; i++) {
    lit.data[i] = other->data[i];
  }
  return lit;
}

__attribute__((always_inline)) inline vd_literal alloc_vdliteral(size_t n) {
  vd_literal lit =
      (vd_literal){.data = (double *)calloc(n, sizeof(double)), .size = n};
  if (NULL == lit.data) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return lit;
}

__attribute__((always_inline)) inline void sprint_vector(gString *inp, const vd_literal *value) {
  if (NULL == value->data) {
    g_append_back_c(inp, "NULL");
    return;
  }
  g_append_back_c(inp, "< ");
  for (size_t i = 0; i < value->size; i++) {
    const size_t buff_size = 17;
    char buff[buff_size];
    int len = snprintf(buff, buff_size - 1, "%lf ", value->data[i]);
    if (0 > len) {
      fprintf(stderr, "Unknown error occurred in snprintf for %s.\n", __func__);
      exit(1);
    }
    g_append_back(inp, buff, len);
  }
  g_append_back_c(inp, ">");
}

