#include <stdio.h>

#include "../src/functional.h"
#include "../src/operations.h"

void test_evaluated() {
  // x-squared - operation POW [x,2]
  struct Operation* x_squared = a_new_operation();
  *x_squared = a_init(2);
  // sub_operations is NULL by default. y=x
  x_squared->sub_operations[0] = NULL;
  // sub_operations initialized with default value 2 & no sub_ops
  x_squared->sub_operations[1] = a_new_operation();
  x_squared->sub_operations[1]->_default = 2;

  x_squared->function = &a_pow;
  
  double one = 1, neg = -1, three = 3, rand = 2480.3;
  double one_squared = a_evaluate(x_squared, &one);
  double negative_squared = a_evaluate(x_squared, &neg);
  double three_squared = a_evaluate(x_squared, &three);
  double random_squared = a_evaluate(x_squared, &rand);

  a_destroy_recursive(x_squared);
  
  printf("%lf * %lf = %lf ?: \n", one, one, one_squared);
  if (one * one != one_squared) printf("FAIL: test_evaluated(1), answer = %lf expected = %lf\n", one_squared, one * one);
  printf("%lf * %lf = %lf ?: \n", neg, neg, negative_squared);
  if (neg * neg != negative_squared) printf("FAIL: test_evaluated(2), answer = %lf expected = %lf\n", negative_squared, neg * neg);
  printf("%lf * %lf = %lf ?: \n", three, three, three_squared);
  if (three * three != three_squared) printf("FAIL: test_evaluated(3), answer = %lf expected = %lf\n", three_squared, three * three);
  printf("%lf * %lf = %lf ?: \n", rand, rand, random_squared);
  if (rand * rand != random_squared) printf("FAIL: test_evaluated(4), answer = %lf expected = %lf\n", random_squared, rand);
}


int main() {
  test_evaluated();
  
  return 0;
}
