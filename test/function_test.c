#include <stdio.h>

#include "../src/functional/function.h"
#include "../src/utils/elementary.h"

#include "helpers/testing.h"

void test_identity() {
  printf("Testing identity\n");
  struct Operation* identity = NULL;
  for(double x = -10.0; x < 10.0; x = x + 2.45) {
    double y = a_evaluate(identity, x);
    EQUALS(x,y);
  }
  a_free(identity);
}

void test_constant() {
  printf("Testing constant\n");
  struct Operation* constt = a_operation(1);
  for (double x = -12.2; x < 2.45; x = x + 1.8) {
    double y = a_evaluate(constt, x);
    EQUALS(constt->_default,y);
  }
  constt->_default = 256.2;
  for (double x = -35.2; x < 2.45; x = x + 5.6) {
    double y = a_evaluate(constt, x);
    EQUALS(constt->_default,y);
  }

  a_free(constt);
}

void test_evaluated() {
  // x-squared - operation POW [x,2]
  struct Operation* x_squared = a_operation(1);
  struct Operation* two = a_operation(1);
  two->_default = 2.0;

  x_squared->binary_function = &b_pow;
  
  x_squared->_right = two;

  double one = 1, neg = -1, three = 3, rand = 2480.3;
  double one_squared = a_evaluate(x_squared, one);
  double negative_squared = a_evaluate(x_squared, neg);
  double three_squared = a_evaluate(x_squared, three);
  double random_squared = a_evaluate(x_squared, rand);

  a_free_recursive(x_squared);
  
  EQUALS(one * one, one_squared);
  EQUALS(neg * neg, negative_squared);
  EQUALS(three * three, three_squared);
  EQUALS(rand * rand, random_squared);
}


int main() {
  test_identity();
  test_constant();
  test_evaluated();
  
  return 0;
}
