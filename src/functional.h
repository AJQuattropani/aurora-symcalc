#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef double (*function_t)(const double*, size_t);

struct Operation {
  // operation performed on inputs
  function_t function;
  // suboperations pointed to to derive inputs
  struct Operation** sub_operations;
  // 0, 1, 2
  size_t num_subops;
  // default value for output
  double _default;
};

/**
  * Initializes an empty Operation.
  */
struct Operation a_init(size_t num_subops);

/**
  * Initializes a tuple containing addresses to empty sub-operations.
  */
struct Operation** a_init_sub(size_t num_subops);

/**
 * Performs heap allocation for an Operation.
 */
struct Operation *a_new_operation();

/**
  * Resets the Operation, frees all memory buffers directly pointed to.
  * Will lead to memory leak if branches are not already managed.
  * Frees the pointer.
  */
void a_destroy(struct Operation *operation);

/**
  * Resets the Operation. Handles indirect memory by freeing it.
  * Function is responsible for freeing pointer.
  */
void a_destroy_recursive(struct Operation *operation);

/**
  * Evaluates recursively from its branches up to its root.
  */
double a_evaluate(const struct Operation *operation, const double *x);





