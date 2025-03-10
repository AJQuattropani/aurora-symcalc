#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "operation.h"

/**
* Initializes a buffer of operations.
*/
struct Operation *a_operation(size_t n);

/**
* Frees the data without destroying branches.
*/
void a_free(struct Operation *operation);

/**
* Frees the operation and recursively destroys all sub-branches.
*/
void a_free_recursive(struct Operation *operation);

/**
* Evaluates recursively from its branches up to its root.
*/
double a_evaluate(const struct Operation *operation, double x);
