#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double identity(const double a, const double b) { return a; }

double add(const double a, const double b) { return a + b; }

double mult(const double a, const double b) { return a * b; }

typedef double (*function_t)(double, double);
struct Function {
  // double(**steps)(double,double);
  function_t *steps;
  size_t size;
};

void print(struct Function *func) {
  for (size_t i = 0; i < func->size; i++) {
    //printf("%lu : %p\n", i, (void *)(func->steps[i]));
  }
}

struct Function init(size_t size) {
  struct Function func;

  func.size = size;

  //size_t block_size = size * sizeof(function_t);

  func.steps = (function_t *)calloc(size, sizeof(function_t));
  if (func.steps == NULL)
    exit(-1);

  for (size_t i = 0; i < size; i++) {
    func.steps[i] = &identity;
  }

  return func;
}

void destroy(struct Function *func) {
  func->size = 0;
  free(func->steps);
  func->steps = NULL;
}

struct Function build_function(size_t size, char *args[]) {
  struct Function func = init(size);

  for (size_t i = 0; i < size; i++) {
    if (strncmp(args[i], "add", 3) == 0) {
      func.steps[i] = &add;
      continue;
    }
    if (strncmp(args[i], "mult", 4) == 0) {
      func.steps[i] = &mult;
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

int main(int argc, char *argv[]) {
  //printf("Identity address: %p | Add address : %p | Mult address: %p \n",
  //       &identity, &add, &mult);

  if (argc < 2)
    return -1;

  printf("Building function with %d steps.\n", argc - 1);
  struct Function func = build_function(argc - 1, argv + 1);

  print(&func);

  double x = 0;
  char buffer[64];

  while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    if (buffer[0] == '\n' || buffer[0] == '\0') break;
    sscanf(buffer, "%lf", &x);
    double y = evaluate(&func, x);
    printf("Output: %lf\n", y);
  }

  destroy(&func);

  return 0;
}
