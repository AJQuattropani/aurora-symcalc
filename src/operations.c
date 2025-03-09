#include "operations.h"

double a_add(const double *args, size_t num_subops) {
  double y = 0;
  size_t i = num_subops;
  do {
    i--;
    y = y + args[i];
  } while (i > 0);
  return y;
}

double a_mult(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--;
    y = y * args[i];
  } while (i > 0);
  return y;
}

double a_pow(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--;
    y = pow(args[i], y);
  } while (i > 0);
  return y;
}

double a_sin(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--; // sine on multiple inputs defined as multiplication of sines
    y = y * sin(args[i]);
  } while (i > 0);
  return y;
}

double a_cos(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--; // sine on multiple inputs defined as multiplication of sines
    y = y * cos(args[i]);
  } while (i > 0);
  return y;
}

double a_log(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--;
    y = log(args[i]) / y;
  } while (i > 0);
  return y;
}

double a_tan(const double *args, size_t num_subops) {
  return a_sin(args, num_subops) / a_cos(args, num_subops);
}

double a_sec(const double *args, size_t num_subops) {
  return 1.0 / a_cos(args, num_subops);
}

double a_csc(const double *args, size_t num_subops) {
  return 1.0 / a_sin(args, num_subops);
}

double a_cot(const double *args, size_t num_subops) {
  return a_cos(args, num_subops) / a_sin(args, num_subops);
}

double a_arcsin(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--; // sine on multiple inputs defined as multiplication of sines
    y = y * asin(args[i]);
  } while (i > 0);
  return y;
}

double a_arccos(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--; // sine on multiple inputs defined as multiplication of sines
    y = y * acos(args[i]);
  } while (i > 0);
  return y;
}

double a_arctan(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--; // sine on multiple inputs defined as multiplication of sines
    y = y * atan(args[i]);
  } while (i > 0);
  return y;
}

double a_arccot(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--; // sine on multiple inputs defined as multiplication of sines
    y = y * atan(1.0 / args[i]);
  } while (i > 0);
  return y;
}

double a_arcsec(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--; // sine on multiple inputs defined as multiplication of sines
    y = y * acos(1.0 / args[i]);
  } while (i > 0);
  return y;
}

double a_arccsc(const double *args, size_t num_subops) {
  double y = 1;
  size_t i = num_subops;
  do {
    i--; // sine on multiple inputs defined as multiplication of sines
    y = y * asin(1.0 / args[i]);
  } while (i > 0);
  return y;
}

