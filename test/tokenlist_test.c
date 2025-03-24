#include "../src/tokenlist.h"

void print_id(const struct token_node* tok) {
  printf("ID: %d | ", tok->token.id);
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

  empty_list(&list);

  return 0;
}


