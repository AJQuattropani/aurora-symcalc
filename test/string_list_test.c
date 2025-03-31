#include <stdio.h>
#include "../src/utils/string_list.h"


int main() {
  struct stringview_list list = new_list(2);
  char* a = "alpha", *b = "beta", *d = "delta";
  
  push_view(&list, a);
  push_view(&list, b);
  push_view(&list, d);

  for(size_t i = 0; i < list.size; i++) {
    printf("%s", list.data[i]);
  }

  empty_stringlist(&list);
  
  push_view(&list, a);
  push_view(&list, d);
  push_view(&list, d);
  for(size_t i = 0; i < list.size; i++) {
    printf("%s", list.data[i]);
  }

  free_stringlist(&list);

  return 0;
}


