#include "graph.h"

double evaluate(const struct Operation *operation, const double *x) {
  // if operation is null, indicates end of branch and to end recursion.
  if (operation == NULL)
    return *x;

  // if operation is constant, ergo no function or inputs, return the
  // constant/default value.
  if (operation->sub_operations == NULL || operation->num_subops == 0 ||
      operation->function == NULL)
    return operation->_default;

  // buffer for function inputs to load in to
  double *inputs = (double *)calloc(operation->num_subops, sizeof(double));
  if (inputs == NULL) {
    fprintf(stderr, "Double buffer allocation failed.");
    exit(-1);
  }

  // instruct sub-operations to compute inputs
  for (size_t i = 0; i < operation->num_subops; i++) {
    inputs[i] = evaluate(&operation->sub_operations[i], x);
  }

  // compute output
  double output = operation->function(inputs, operation->num_subops);
  free(inputs);
  return output;
}

void _free_recursive(struct Operation *operation) {
  if (operation == NULL)
    return;
  operation->function = NULL;
  operation->num_subops = 0;
  if (operation->sub_operations != NULL) {
    _free_recursive(&operation->sub_operations[0]);
    _free_recursive(&operation->sub_operations[1]);
  }
  free(operation->sub_operations);
  operation->sub_operations = NULL;
}

void _free(struct Operation *operation) {
  if (operation == NULL)
    return;
  operation->function = NULL;
  operation->num_subops = 0;
  free(operation->sub_operations);
  operation->sub_operations = NULL;
}

struct Operation _init() {
  struct Operation base;
  base.num_subops = 0;
  base._default = 1;
  base.function = NULL;
  base.sub_operations = _init_sub();
  return base;
}


struct Operation* _init_sub() {
  struct Operation* doublet = (struct Operation*)calloc(2, sizeof(struct Operation));
  if (doublet == NULL) {
    fprintf(stderr, "Operations buffer allocation failed.");
    exit(-1);
  }
  return doublet;
}
