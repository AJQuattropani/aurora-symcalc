#include "../src/tokenlist.h"

void print_id(const struct token_list* tok) {
  printf("ID: %d | ", tok->token.id);
}

int main() {
  struct token_list* base = new_list(); 
  
  struct token a = {.id = 30}, b = {.id = 24}, c = {.id = 21};
  emplace_front(&base, a);
  emplace_front(&base, b);
  emplace_front(&base, c);
  
  struct token_list* appendix = new_list();
  
  struct token x = {.id = 20};
  emplace_front(&appendix, x);

  combine_front(&base, appendix);


  for_each(base, &print_id);
  

  free_list(base);

  return 0;
}


