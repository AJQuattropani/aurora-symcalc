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

Object read_scalar(const token_array *args) {
  if (args->size == 1) return read_scalar_imp(&args->data[0]);
  return null_object();
}

Object read_vector(const token_array *args) {
  vd_literal vector = alloc_vdliteral(args->size);
  for (size_t i = 0; i < args->size; i++) {
    token *curr = &args->data[i];
    if (try_read_double(&vector.data[i], &curr->token->key)) {
      fprintf(stderr,
              "[SKIPPED] Non-double definition provided for input %ld.\n", i);
      vector.data[i] = 0;
      continue;
    }
  }
  return as_vdliteral(&vector);
}

