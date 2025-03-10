#pragma once

#include <math.h>
#include <stdio.h>

const double EPS = 1E-14;

#define ASSERT(x)                                                              \
  if (!(x)) {                                                                  \
    printf("Assert failed at %s#%d || %s\n", __FILE__, __LINE__, #x);                    \
  }

#define EQUALS(x,y) { \
  double err; int result = dbl_err_cmp(x,y,&err);  \
  if (!result) { \
    printf("[FAILED] Equality test at %s$%d || %lf =/= %lf\n", __FILE__, __LINE__, x, y); \
    printf("---dbl_err_cmp returned %d and err = %lf\n", result, err); \
  }}

static inline int dbl_err_cmp(double expec, double real, double* err) {
  *err = fabs((real - expec) / expec);
  return (*err < EPS) || (real < EPS && expec < EPS);
}
