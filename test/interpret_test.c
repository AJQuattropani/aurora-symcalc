#include "../src/interpret.h"

#include <string.h>

//#include "helpers/testing.h"

int main() {
  char* string = "x^2-(1+2.5(x/2.2)^3)-3.3/x\n";
  //build_function(string, strlen(string));
  
  printf("%s", string);

  struct token_list tokens = parse_string_tokens(string, strlen(string));
  
  print(&tokens);

  empty_list(&tokens);

  return 0;
}

