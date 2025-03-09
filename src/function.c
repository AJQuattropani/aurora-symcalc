#include "function.h"


struct Function init(size_t size) {
  struct Function func;

  func.size = size;

  //size_t block_size = size * sizeof(function_t);

  func.steps = (function_t *)calloc(size, sizeof(function_t));
  if (func.steps == NULL)
    exit(-1);

  for (size_t i = 0; i < size; i++) {
    func.steps[i] = &_identity;
  }
  return func;
}

void destroy(struct Function *func) {
  func->size = 0;
  free(func->steps);
  func->steps = NULL;
}

void print(struct Function *func) {
  for (size_t i = 0; i < func->size; i++) {
    //printf("%lu : %p\n", i, (void *)(func->steps[i]));
  }
}


struct Function build_function(size_t size, char *args[]) {
  struct Function func = init(size);

  for (size_t i = 0; i < size; i++) {
    if (strncmp(args[i], "add", 3) == 0) {
      func.steps[i] = &_add;
      continue;
    }
    if (strncmp(args[i], "mult", 4) == 0) {
      func.steps[i] = &_mult;
      continue;
    }
    if (strncmp(args[i], "pow", 3) == 0) {
      func.steps[i] = &_pow;
      continue;
    }

  }
  return func;
}

double evaluate(const struct Function *func, double x) {
  for (size_t i = 0; i < func->size; i++) {
    function_t step = func->steps[i];
    if (step == NULL)
      exit(-1);
    x = step(x, x);
  }
  return x;
}





