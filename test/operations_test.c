#include <stdio.h>

#include "../src/operations.h"

void test_add()
{
  double a = 1.0;
  double b = 2.0;
  double c = 4.2;
  double d = -140;
  double nums[4] = {a,b,c,d};

  double sum = a_add(&nums[0], 4);
  if (sum != a+b+c+d)
  {
    printf("test_add failed.\n");
    return;
  }
  
  printf("test_add() passed.\n");
}

void test_mult()
{
  double a = 8.4551;
  double b = 4.0;
  double c = 3.1415;
  double d = -69;
  double e = -24421;
  double nums[5] = {a,b,c,d,e};

  double prod = a_mult(&nums[0], 5);
  double ans = a*b*c*d*e;
  const double EPS = 0.000000001;
  double err = fabs(prod-ans);
  if (fabs(prod-ans) < EPS) {
    printf("FAILED test_mult(1), a_mult yieled %lf | expected: %lf \n", prod, a*b*c*d*e);
    return;
  }
  printf("INFO: test_mult(1) error was %lf \n", err);

  prod = a_mult(&nums[0], 1);
  if (prod != a) {
    printf("test_mult(2) failed.\n");
    return;
  }

  prod = a_mult(&nums[2], 3);
  if (prod != c*d*e) {
    printf("test_mult(3) failed.\n");
    return;
  }

  printf("test_mult passed.\n");
}


int main() {
  test_add();
  test_mult();
  return 0;
}


