#include "operators.h"

__attribute__((always_inline)) inline vector_size_t flag_scalar(vector_size_t i) {
  return SCALAR != i;
}

int vb_add(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  for (vector_size_t i = 0; i < o->size; i++) {
    o->data[i] = l->data[sc_left * i] + r->data[sc_right * i];
  }
  return 0;
}
int vb_sub(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  for (vector_size_t i = 0; i < o->size; i++) {
    o->data[i] = l->data[sc_left * i] - r->data[sc_right * i];
  }
  return 0;
}

int vb_mul(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  for (vector_size_t i = 0; i < o->size; i++) {
    o->data[i] = l->data[sc_left * i] * r->data[sc_right * i];
  }
  return 0;
}

int vb_div(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  for (vector_size_t i = 0; i < o->size; i++) {
    o->data[i] = l->data[sc_left * i] / r->data[sc_right * i];
  }
  return 0;
}

int vb_pow(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  for (vector_size_t i = 0; i < o->size; i++) {
    fprintf(stderr, " %lf ^ %lf", l->data[sc_left * i], r->data[sc_right * i]);
    o->data[i] = pow(l->data[sc_left * i], r->data[sc_right * i]);
    fprintf(stderr, " %lf |", o->data[i]);
  }
  return 0;
}

int vb_log(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  for (vector_size_t i = 0; i < o->size; i++) {
    o->data[i] = log(r->data[sc_right * i]) / log(l->data[sc_left * i]);
  }
  return 0;
}

int vu_set(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = i->data[sc_left * j];
  }
  return 0;
}

int vu_neg(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = -1.0 * i->data[sc_left * j];
    fprintf(stderr, "%lf |", o->data[j]);
  }
  return 0;
}

int vu_sin(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = sin(i->data[sc_left * j]);
  }
  return 0;
}

int vu_cos(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = cos(i->data[sc_left * j]);
  }
  return 0;
}

int vu_tan(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = tan(i->data[sc_left * j]);
  }
  return 0;
}

int vu_sec(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = 1.0/cos(i->data[sc_left * j]);
  }
  return 0;
}

int vu_csc(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = 1.0/sin(i->data[sc_left * j]);
  }
  return 0;
}

int vu_cot(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = 1.0/tan(i->data[sc_left * j]);
  }
  return 0;
}


int vu_asin(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = asin(i->data[sc_left * j]);
  }
  return 0;
}

int vu_acos(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = acos(i->data[sc_left * j]);
  }
  return 0;
}

int vu_atan(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = atan(i->data[sc_left * j]);
  }
  return 0;
}

int vu_log(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_left = flag_scalar(i->size);
  for (vector_size_t j = 0; j < o->size; j++) {
    o->data[j] = log(i->data[sc_left * j]);
  }
  return 0;
}



