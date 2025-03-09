#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>

#include "fmath.h"

typedef double (*function_t)(double, double);
struct Function {
  // double(**steps)(double,double);
  function_t *steps;
  size_t size;
};



/**
 * Returns the identity function with n entries.
  */
struct Function init(size_t n);

/**
 * Performs memory cleanup on the function.
 */
void destroy(struct Function *func);

/**
* Prints the steps of the function.
 */
void print(struct Function* func);

/**
* Builds a new functiom from a set strings encoding instructions.
*/
struct Function build_function(size_t size, char* args[]);

/**
* Evaluates the function from input variable x.
*/
double evaluate(const struct Function *func, double x);





