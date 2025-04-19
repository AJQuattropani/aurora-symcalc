#include "vector.h"

Object as_vdliteral(vd_literal *value) {
  return (Object){.vLiteral = *value, .ty = VECTOR};
}

void free_vdliteral(vd_literal *lit) {
  if (NULL != lit->data) {
    free(lit->data);
  }
  lit->size = 0;
}

vd_literal alloc_vdliteral(size_t n) {
  vd_literal lit =
      (vd_literal){.data = (double *)calloc(n, sizeof(double)), .size = n};
  if (NULL == lit.data) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return lit;
}

