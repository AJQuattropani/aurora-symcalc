#include "../src/interpret.h"
#include "helpers/testing.h"

//#include "helpers/testing.h"

void test_for(const char* string)
{
  printf("%s\n", string);
  struct token_list tokens = parse_string_tokens(string, strlen(string));
  print(&tokens);
  empty_list(&tokens);
}

void test_build()
{
  {
  const char* string = "2_x+\\sin(x-2.4)";
  printf("%s\n", string);
  
  struct token_list tokens = parse_string_tokens(string, strlen(string));
  print(&tokens);
  struct Operation *oper = least_significant_operation(tokens.head, NULL);

  double x = 2.3;
  double y = a_evaluate(oper, x);
  
  double y_exp = b_log(2,x) + u_sin(b_sub(x,2.4));
  EQUALS(y, y_exp);
  a_free_recursive(oper);
  empty_list(&tokens);
  }
  {
  const char* string2 = "x_2 + e_(2\\sin(x-1.5))-\\arctan(x/2)^(2-x)x";
  printf("%s\n", string2);
  
  struct token_list tokens = parse_string_tokens(string2, strlen(string2));
  print(&tokens);
  
  struct Operation *oper = least_significant_operation(tokens.head, NULL);

  double x = 2;
  double y = a_evaluate(oper, x);
  
  double y_exp = b_add(b_log(x,2), 
                       b_sub(b_log(2.718281828459045,2*u_sin(b_sub(x,1.5))), b_mult(b_pow(u_arctan(b_div(x,2)),b_sub(2,x)), x)));
  EQUALS(y, y_exp);
  a_free_recursive(oper);
  empty_list(&tokens);
  }
  {
  const char* string3 = "x";
  printf("%s\n", string3);
  struct token_list tokens = parse_string_tokens(string3, strlen(string3));
  
  print(&tokens);
  
  struct Operation *oper = least_significant_operation(tokens.head, NULL);

  double x = -596922;
  double y = a_evaluate(oper, x);
  
  double y_exp = x;
  EQUALS(y, y_exp);
  a_free_recursive(oper);
  empty_list(&tokens);
  }
  {
  const char* string3 = "2";
  printf("%s\n", string3);
  struct token_list tokens = parse_string_tokens(string3, strlen(string3));
  
  print(&tokens);
  
  struct Operation *oper = least_significant_operation(tokens.head, NULL);

  double x = -596922.455;
  double y = a_evaluate(oper, x);
  
  double y_exp = 2;
  EQUALS(y, y_exp);
  a_free_recursive(oper);
  empty_list(&tokens);
  }

}

int main() {
  //test_for("x^2-(1+2.5(x/2.2)^3)-3.3/x\n");
  //test_for("(x+1.1^1.1)(e^x-x*x)  - 2.2 \n");
  //test_for("x_2 + 13- \\sin(x) + \\arctan(x)");
  //test_for("2x + 13.45- \\sin(e^2) + \\arctan(x)");
  //test_for("x_2(x) + 13- \\sin(x + \\arctan(x)");
  //test_for("x_2 + 13- \\sinx) + \\arctanx ");
  //test_for("x_2 + e_(2\\sin(x-1.5))-\\arctan(x/2)^(2-x)x");
  
  test_build();

  return 0;
}

