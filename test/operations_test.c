#include <stdio.h>

#include "../src/operations.h"

const double EPS = 0.000000000001;

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
  double err = fabs((prod-ans)/ans);
  printf("INFO: test_mult(1) error was %lf \n", err);
  if (err >= EPS) {
    printf("FAILED test_mult(1), a_mult yieled %lf | expected: %lf \n", prod, ans);
    return;
  }


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

void test_log()
{
  double a = 345.24;
  double b = 250.0;
  double c = 2.2;
  double d = 29;
  double e = 0.45;

  double nums[5] = {a,b,c,d,e};

  double ans = log(a) * log(c) * log(e) / (log(b) * log(d));
  
  double prod = a_log(&nums[0], 5);
  double err = fabs((prod-ans)/ans);
  printf("INFO: test_log(1) error was %lf \n", err);
  if (err >= EPS) {
    printf("FAILED test_log(1), a_log yieled %lf | expected %lf \n", prod, ans);
    return;
  }

  printf("test_log passed.\n");

}


int main() {
  test_add();
  test_mult();
  test_log();

  return 0;
}


