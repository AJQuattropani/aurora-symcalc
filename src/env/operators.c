#include "operators.h"


__attribute__((always_inline)) inline vector_size_t flag_scalar(vector_size_t i) {
  return SCALAR != i;
}

int is_num(const vd_literal *check, double val, double tol) {
  vector_size_t sc_check = flag_scalar(check->size);
  vector_size_t size = (check->size - 1) * (sc_check) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    if (fabs(check->data[j] - val) <= tol) {
      return 1;
    }
  }
  return 0;
}




int vb_add(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = l->data[sc_left * j] + r->data[sc_right * j];
  }
  return 0;
}

int vb_sub(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = l->data[sc_left * j] - r->data[sc_right * j];
  }
  return 0;
}

int vb_mul(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = l->data[sc_left * j] * r->data[sc_right * j];
  }
  return 0;
}

int vb_div(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = l->data[sc_left * j] / r->data[sc_right * j];
  }
  return 0;
}

int vb_pow(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = pow(l->data[sc_left * j], r->data[sc_right * j]);
  }
  return 0;
}

int vb_log(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(l->size);
  vector_size_t sc_right = flag_scalar(r->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = log(r->data[sc_right * j]) / log(l->data[sc_left * j]);
  }
  return 0;
}

int vu_set(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = i->data[sc_left * j];
  }
  return 0;
}

int vu_neg(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = -1.0 * i->data[sc_left * j];
  }
  return 0;
}

int vu_sin(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = sin(i->data[sc_left * j]);
  }
  return 0;
}

int vu_cos(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = cos(i->data[sc_left * j]);
  }
  return 0;
}

int vu_tan(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = tan(i->data[sc_left * j]);
  }
  return 0;
}

int vu_sec(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = 1.0/cos(i->data[sc_left * j]);
  }
  return 0;
}

int vu_csc(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = 1.0/sin(i->data[sc_left * j]);
  }
  return 0;
}

int vu_cot(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = 1.0/tan(i->data[sc_left * j]);
  }
  return 0;
}


int vu_asin(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = asin(i->data[sc_left * j]);
  }
  return 0;
}

int vu_acos(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = acos(i->data[sc_left * j]);
  }
  return 0;
}

int vu_atan(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = atan(i->data[sc_left * j]);
  }
  return 0;
}

int vu_log(vd_literal *o, const vd_literal *i) {
  vector_size_t sc_out = flag_scalar(o->size);
  vector_size_t sc_left = flag_scalar(i->size);
  vector_size_t size = (o->size - 1) * (sc_out) + 1;
  for (vector_size_t j = 0; j < size; j++) {
    o->data[j] = log(i->data[sc_left * j]);
  }
  return 0;
}



