#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef double (*function_t)(const double*, size_t num_subops);

struct Operation {
  // operation performed on inputs
  function_t function;
  // suboperations pointed to to derive inputs
  struct Operation* sub_operations;
  // 0, 1, 2
  size_t num_subops;
  // default value for output
  double _default;
};

/**
  * Initializes an empty Operation. 
  */
struct Operation _init();

/**
  * Initializes a doublet containing the sub-operations.
  */
struct Operation* _init_sub();

/**
  * Resets the Operation.
  * Maybe be unsafe if branch nodes are not cleaned up first.
  */
void _free(struct Operation *operation);

/**
  * Resets the Operation, including all branches.
  */
void _free_recursive(struct Operation *operation);

/**
  * Evaluates recursively from its branches up to its root.
  */
double evaluate(const struct Operation *operation, const double *x);
