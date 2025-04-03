#include "../src/functional/tokenlist.h"

void print_id(const struct token_node* tok) {
  printf("ID: %d\n", tok->token.id);
}

int main() {
  struct token a = {CNS, 4, {.cnst = 35.2}},
  b = {IDX, 3, {.id='x'}},
  c = {UNR, 2, {.id='+'}};

  struct token_list list = {};
  emplace_back(&list, &a);
  emplace_back(&list, &b);
  emplace_back(&list, &c);
  
  print(&list);
  
  //for_each(&list, &print_id);

  empty_list(&list);

  return 0;
}


