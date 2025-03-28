#include "../src/interpret.h"

#include <string.h>

//#include "helpers/testing.h"

void test_for(const char* string)
{
  printf("%s\n", string);
  struct token_list tokens = parse_string_tokens(string, strlen(string));
  print(&tokens);
  empty_list(&tokens);
}

int main() {
  test_for("x^2-(1+2.5(x/2.2)^3)-3.3/x\n");
  test_for("(x+1.1^1.1)(e^x-x*x)  - 2.2 \n");
  test_for("x_2 + 13- \\sin(x) + \\arctan(x)");
  test_for("2x + 13.45- \\sin(e^2) + \\arctan(x)");
  test_for("x_2(x) + 13- \\sin(x + \\arctan(x)");
  test_for("x_2 + 13- \\sinx) + \\arctanx ");
  test_for("x_2 + 13- \\ssnx ");

  return 0;
}

