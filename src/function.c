#include "function.h"



struct Operation *a_operation(size_t n) {
  struct Operation *operation =
      (struct Operation *)calloc(n, sizeof(struct Operation));
  if (operation == NULL) {
    fprintf(stderr, "Operations buffer allocation failed.");
    exit(-1);
  }
  return operation;
}

void a_free(struct Operation *operation) {
  // protection against writing/freeing to null
  if (operation == NULL)
    return;
  free(operation);
  operation = NULL;
}

void a_free_recursive(struct Operation *operation) {
  if (operation == NULL)
    return;
  // free left and right branches
  a_free_recursive(operation->_left);
  a_free_recursive(operation->_right);

  free(operation);
  operation = NULL;
}

double a_evaluate(const struct Operation *operation, double x) {
  // BASE CASE 1: no operation - identity
  if (operation == NULL)
    return x;

  //
  mon_function_t mono = operation->post_function;
  if (mono == NULL)
    mono = &identity;

  // BASE CASE 2: operation exists with no dependencies - constant
  if ((operation->_left == NULL && operation->_right == NULL) ||
      operation->binary_function == NULL)
    return mono(operation->_default);

  double in_left = a_evaluate(operation->_left, x);
  double in_right = a_evaluate(operation->_right, x);

  double y = operation->binary_function(in_left, in_right);
  return mono(y);
}

