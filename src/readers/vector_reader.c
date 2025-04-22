#include "vector_reader.h"

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

__attribute__((always_inline)) inline int try_read_double(double* target, const mString *mstr) {
    return 0 >= sscanf(mstr->cstring, "%lf", target);
}

