#include "functional.h"

double a_evaluate(const struct Operation *operation, const double *x) {
  // BASE CASE 1: no operation, return identity.
  if (operation == NULL)
    return *x;

  // BASE CASE 2: operation exists with no dependencies, return default constant value.
  if (operation->sub_operations == NULL || operation->num_subops == 0 ||
      operation->function == NULL)
    return operation->_default;

  // buffer for function inputs to load in to
  double *inputs = (double*)calloc(operation->num_subops, sizeof(double));
  if (inputs == NULL) {
    fprintf(stderr, "Double buffer allocation failed.");
    exit(-1);
  }

  // instruct sub-operations to compute inputs
  for (size_t i = 0; i < operation->num_subops; i++) {
    inputs[i] = a_evaluate(operation->sub_operations[i], x);
  }

  // compute output
  double output = operation->function(inputs, operation->num_subops);
  free(inputs);
  return output;
}

void a_destroy_recursive(struct Operation *operation) {
  // protection against writing to null
  if (operation == NULL)
    return;
  if (operation->sub_operations != NULL) {
    //recursive deletion should always be done opposite-direction of recursive execution
    size_t i = operation->num_subops;
    do {
      i--;
      a_destroy_recursive(operation->sub_operations[i]);
    } while (i > 0);
    free(operation->sub_operations);
    operation->sub_operations = NULL;
  }
  
  // may be gratuitous
  operation->function = NULL;
  operation->num_subops = 0;
  
  free(operation);
  operation = NULL;
}

void a_destroy(struct Operation *operation) {
  // protection against writing/freeing to null
  if (operation == NULL)
    return;
  
  // if nodes pointed to by sub_operations are not already handled, this free
  // will cause a memory leak.
  if (operation->sub_operations != NULL) {
    free(operation->sub_operations);
    operation->sub_operations = NULL;
  }
  
  // may be gratuitous
  operation->function = NULL;
  operation->num_subops = 0;

  free(operation);
  operation = NULL;
}

struct Operation a_init(size_t num_subops) {
  struct Operation base;
  base.num_subops = num_subops;
  base._default = 1;
  base.function = NULL;
  base.sub_operations = a_init_sub(num_subops);
  return base;
}


struct Operation** a_init_sub(size_t num_subops) {
  struct Operation** tuple = (struct Operation**)calloc(num_subops, sizeof(struct Operation*));
  if (tuple == NULL) {
    fprintf(stderr, "SubOperations pointer buffer allocation failed.");
    exit(-1);
  }
  return tuple;
}
struct Operation *a_new_operation() {
  struct Operation* operation = (struct Operation*)calloc(1, sizeof(struct Operation));
  if (operation == NULL) {
    fprintf(stderr, "SubOperations buffer allocation failed.");
    exit(-1);
  }
  return operation;
}

