#pragma once

typedef double (*bin_function_t)(const double, const double);
typedef double (*mon_function_t)(const double);

struct Operation {
  bin_function_t binary_function;
  mon_function_t post_function;
  
  struct Operation* _left;
  struct Operation* _right;

  double _default;
};

static inline double identity(double x)
{
  return x;
}

