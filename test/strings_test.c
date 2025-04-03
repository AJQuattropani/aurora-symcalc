#include "../src/utils/strings.h"

int main() {
  struct string str = new_string(15);
  
  const char* lit1 = "lorem ipsum";
  const char* lit2 = "four score and six";
  
  append_back(&str, lit1, strlen(lit1));
  append_back(&str, lit1, strlen(lit1));

  printf("%s\n", str.data);

  empty_string(&str);
  
  append_back(&str, lit2, strlen(lit2));
  append_back(&str, lit2, strlen(lit2));
  append_back(&str, lit1, strlen(lit1));

  printf("%s\n", str.data);

  free_string(&str);
}


