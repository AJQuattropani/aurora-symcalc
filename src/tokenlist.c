#include "tokenlist.h"

struct token_list *new_list() {
  struct token_list *list =
      (struct token_list *)calloc(1, sizeof(struct token_list));
  if (list == NULL)
    exit(-1);
  return list;
}

void free_list(struct token_list *list) {
  if (list == NULL) {
    return;
  }
  free_list(list->next);
  free(list);
  list = NULL;
}

void for_each(struct token_list *list, void (*method)(const struct token_list *)) {
  for (struct token_list *ptr = list; ptr != NULL; ptr = ptr->next)
    method(ptr);
}

void emplace_front(struct token_list **list, struct token token) {
  struct token_list *appendix = (struct token_list*)malloc(sizeof(struct token_list));
  if (appendix == NULL) {
    fprintf(stderr, "Emplace malloc failed.");
    exit(-1);
  }
  memcpy(&appendix->token, &token, sizeof(struct token));
  appendix->next = *list; 
  *list = appendix;
}

void combine_front(struct token_list **list, struct token_list *appendix) {
  if (appendix == NULL) return;
  struct token_list *last_ptr;
  for (last_ptr = appendix; last_ptr->next != NULL; last_ptr = last_ptr->next);
  last_ptr->next = *list;
  *list = appendix;
}

void print(const struct token_list *tok) {
  const struct token *t = &tok->token;
  printf("{%ld,", t->priority);
  if(t->type != CNS) printf("%c}, ", t->id);
  else
  printf("%lf}, ", t->cnst);
}
