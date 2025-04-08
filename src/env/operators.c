#include "operators.h"

int vb_add(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  if (l->size != r->size && l->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = l->size;
  // godbolt says this should be vectorized by compiler
  for (size_t i = 0; i < size; i++) {
    o->data[i] = l->data[i] + r->data[i];
  }
  return 0;
}
int vb_sub(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  if (l->size != r->size && l->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = l->size;
  // godbolt says this should be vectorized by compiler
  for (size_t i = 0; i < size; i++) {
    o->data[i] = l->data[i] - r->data[i];
  }
  return 0;
}

int vb_mul(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  if (l->size != r->size && l->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = l->size;
  // godbolt says this should be vectorized by compiler
  for (size_t i = 0; i < size; i++) {
    o->data[i] = l->data[i] * r->data[i];
  }
  return 0;
}

int vb_div(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  if (l->size != r->size && l->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = l->size;
  // godbolt says this should be vectorized by compiler
  for (size_t i = 0; i < size; i++) {
    o->data[i] = l->data[i] / r->data[i];
  }
  return 0;
}

int vb_pow(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  if (l->size != r->size && l->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = l->size;
  // godbolt says this should be vectorized by compiler
  for (size_t i = 0; i < size; i++) {
    o->data[i] = pow(l->data[i], r->data[i]);
  }
  return 0;
}

int vb_log(vd_literal *o, const vd_literal *l, const vd_literal *r) {
  if (l->size != r->size && l->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = l->size;
  // godbolt says this should be vectorized by compiler
  for (size_t i = 0; i < size; i++) {
    o->data[i] = log(r->data[i]) / log(l->data[i]);
  }
  return 0;
}

int vu_sin(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size; 

  for (size_t j = 0; j < size; j++) {
    o->data[j] = sin(i->data[j]);
  }
  return 0;
}

int vu_cos(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = cos(i->data[j]);
  }
  return 0;
}

int vu_tan(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = tan(i->data[j]);
  }
  return 0;
}

int vu_sec(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = 1.0/cos(i->data[j]);
  }
  return 0;
}

int vu_csc(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = 1.0/sin(i->data[j]);
  }
  return 0;
}

int vu_cot(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = 1.0/tan(i->data[j]);
  }
  return 0;
}


int vu_asin(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = asin(i->data[j]);
  }
  return 0;
}

int vu_acos(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = acos(i->data[j]);
  }
  return 0;
}

int vu_atan(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = atan(i->data[j]);
  }
  return 0;
}

int vu_log(vd_literal *o, const vd_literal *i) {
  if (i->size != o->size) {
    printf("mismatched inputs for %s, crashing.\n", __func__);
    exit(3);
  }
  size_t size = i->size;
  
  for (size_t j = 0; j < size; j++) {
    o->data[j] = log(i->data[j]);
  }
  return 0;
}



