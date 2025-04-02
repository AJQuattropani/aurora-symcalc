#pragma once

enum Type {
  BEG = -1,
  CNS = 0, // values mapped to reverse-priority
  IDX = 1,
  UNR = 2,
  BIN = 3
};


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

