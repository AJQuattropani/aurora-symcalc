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

vd_literal copy_vdliteral(const vd_literal* other) {
  vd_literal lit = alloc_vdliteral(other->size);
  if (SCALAR == lit.size) {
    lit.data[0] = other->data[0];
    lit.size = other->size; 
    return lit;
  }
  for (vector_size_t i = 0; i < lit.size; i++) {
    lit.data[i] = other->data[i];
  }
  lit.size = other->size; 
  return lit;
}

vd_literal alloc_vdliteral(vector_size_t n) {
  vector_size_t size = (SCALAR == n) * 1 + (SCALAR != n) * n;
  vd_literal lit =
      (vd_literal){.data = (double *)calloc(size, sizeof(double)), .size = n};
  if (NULL == lit.data) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return lit;
}

void sprint_vector(gString *inp, const vd_literal *value) {
  if (SCALAR == value->size) {
    const size_t buff_size = 48;
    char buff[buff_size];
    int len = snprintf(buff, buff_size - 3, "%g", value->data[0]);
    if (0 > len || (size_t)len > buff_size) {
      fprintf(stderr, "Unknown error occurred in snprintf for %s. len %d, %s\n", __func__, len, buff);
      exit(1);
    }
    g_append_back(inp, buff, len);
    return;
  }
  g_append_back_c(inp, "[");
  for (vector_size_t i = 0; i < value->size; i++) {
    const size_t buff_size = 48;
    char buff[buff_size];
    int len = snprintf(buff, buff_size - 3, "%g,", value->data[i]);
    if (0 > len || (size_t)len > buff_size) {
      fprintf(stderr, "Unknown error occurred in snprintf for %s. len: %d, %s\n", __func__, len, buff);
      exit(1);
    }
    g_append_back(inp, buff, len);
  }
  g_append_back_c(inp, "]");
}

vector_list alloc_vdlist(size_t size, vector_size_t ncomponents) {
  vector_size_t components = 1 * (SCALAR == ncomponents) + ncomponents * (SCALAR != ncomponents);
  stack block = new_stack(sizeof(double) * components * size);
  vd_literal *data = (vd_literal *)malloc(sizeof(vd_literal) * size);
  if (NULL == data) {
    fprintf(stderr, "malloc failed in %s.\n", __func__);
    exit(1);
  }
  for (size_t i = 0; i < size; i++) {
    data[i] = (vd_literal){.data = (double *)s_alloc(&block, sizeof(double) * components),
                           .size = ncomponents};
  }
  return (vector_list){.data = data, .size = size};
}


void free_vdlist(vector_list *list) {
  free(list->data[0].data);
  free(list->data);
}

