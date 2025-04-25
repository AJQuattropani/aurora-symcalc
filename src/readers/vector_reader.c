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

void read_linspace(Object *o, const token_array *args) {
  if (args->size > 3) goto argument_size_error;
  double bounds[3] = {NAN,NAN,NAN};
  unsigned long long int partitions;
  for (unsigned char i = 0; i < args->size; i++) {
    const Object *const restrict val = &args->data[i].token->value;
    if (VECTOR != val->ty) goto argument_size_error;
    if (SCALAR != val->vLiteral.size) goto argument_size_error;
    bounds[i] = val->vLiteral.data[0];
  }
  if (isnan(bounds[1]) || isnan(bounds[0])) {
    goto argument_size_error;
  }
  if (isnan(bounds[2]) || bounds[2] <= 0) {
    partitions = 100;
  } else {
    partitions = (unsigned long int)floor(bounds[2]);
    if (partitions != bounds[2]) {
      fprintf(stderr, "[WARN] Decimal value cast to lowest integer.\n");
    }
    if (0 == partitions) goto argument_size_error;
  }
  double a = bounds[0];
  double b = bounds[1];
  double dx = (b - a) / (partitions);
  //fprintf(stdout, "Creating a linspace from %lf to %lf with %lld partitions.\n", a, b, partitions);

  vd_literal linspace = alloc_vdliteral(partitions + 1);
  for (vector_size_t i = 0; i < linspace.size; i++) {
    linspace.data[i] = a + dx * i;
  }
  *o = as_vdliteral(&linspace);
  return;
argument_size_error:
  fprintf(stderr, "[SKIPPED] Insufficient arguments provided for %s\n", __func__);
  *o = null_object();
  return; 
}

void read_countspace(Object *o, const token_array *args) {
  if (args->size > 3) goto argument_size_error;
  double bounds[3] = {NAN,NAN,NAN};
  double dx;
  for (unsigned char i = 0; i < args->size; i++) {
    const Object *const restrict val = &args->data[i].token->value;
    if (VECTOR != val->ty) goto argument_size_error;
    if (SCALAR != val->vLiteral.size) goto argument_size_error;
    bounds[i] = val->vLiteral.data[0];
  }
  if (isnan(bounds[1]) || isnan(bounds[0])) {
    goto argument_size_error;
  }
  if (isnan(bounds[2])) {
    dx = 1;
  } else {
    dx = (bounds[2]);
    //if (0 >= dx) goto argument_size_error;
  }
  double a = bounds[0];
  double b = bounds[1];
  double diff = b - a;
  vector_size_t partitions = diff / dx;
  if (partitions <= 0) goto argument_size_error;

  vd_literal linspace = alloc_vdliteral(partitions + 1);
  for (vector_size_t i = 0; i < linspace.size; i++) {
    linspace.data[i] = a + dx * i;
  }
  *o = as_vdliteral(&linspace);
  return;
argument_size_error:
  fprintf(stderr, "[SKIPPED] Insufficient arguments provided for %s\n", __func__);
  *o = null_object();
  return; 
}

