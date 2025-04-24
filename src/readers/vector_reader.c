#include "vector_reader.h"

__attribute__((always_inline)) inline int try_read_double(double* target, const mString *mstr) {
    return 0 >= sscanf(mstr->cstring, "%lf", target);
}

Object read_scalar_imp(const token *arg) {
  double val;
  if (try_read_double(&val, &arg->token->key)) {
    return null_object();
  }
  vd_literal obj = make_scalar(val);
  return as_vdliteral(&obj);
}

void read_scalar(Object *o, const token_array *args) {
  if (args->size == 1) {
    *o = read_scalar_imp(&args->data[0]);
    return;
  }
  *o = null_object();
}

void read_vector(Object *o, const token_array *args) {
  vd_literal vector = alloc_vdliteral(args->size);
  for (size_t i = 0; i < args->size; i++) {
    token *curr = &args->data[i];
    if (VECTOR == curr->token->value.ty) {
      if (SCALAR == curr->token->value.vLiteral.size) {
        vector.data[i] = curr->token->value.vLiteral.data[0];
        continue;
      }
    }
    if (try_read_double(&vector.data[i], &curr->token->key)) {
      fprintf(stderr,
              "[SKIPPED] Non-double definition provided for input %ld.\n", i);
      vector.data[i] = 0;
      continue;
    }
  }
  *o = as_vdliteral(&vector);
}

