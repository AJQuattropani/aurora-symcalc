#include "../src/interpret.h"

#include <string.h>

//#include "helpers/testing.h"

int main() {
  char* string = "x^2-(1+2.5(x/2.2)^3)-3.3/x\n";

  printf("%s\n", string);

  struct token_list tokens = parse_string_tokens(string, strlen(string));
  
  print(&tokens);
  
  char* string2 = "(x+1.1^1.1)(e^x-x*x)  - 2.2 \n";

  struct token_list tokens2 = parse_string_tokens(string2, strlen(string2));
  print(&tokens2);

  empty_list(&tokens);
  empty_list(&tokens2);
  return 0;
}

