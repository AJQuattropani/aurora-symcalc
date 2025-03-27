#include "tokenlist.h"

struct token_node *_new_node();

/*
 * Internal implementation functions.
 */
void _free_list(struct token_node *node);

void _for_each(struct token_node *first, void (*method)(const struct token_node *));

void _print(const struct token_node *tok);

/*
 * Definition of header functions.
 */
void print(const struct token_list *list) { 
  // TODO use formatters to limit number of prints
  printf("Size of list: %ld\n", list->size);
  for_each(list, _print);
  printf("\n");
}

void for_each(const struct token_list *list,
              void (*method)(const struct token_node *)) {
  _for_each(list->head, method);
}

void empty_list(struct token_list* list) {
  _free_list(list->head);
  list->size = 0;
  list->tail = NULL;
}

void emplace_back(struct token_list* list, const struct token* token) {
  struct token_node *tok = _new_node();
  memcpy(&tok->token, token, sizeof(struct token));
  
  list->size++;

  if (list->size == 0 || list->tail == NULL || list->head == NULL) {
    list->tail = tok;
    list->head = tok;
    return;
  }

  list->tail->next = tok;
  list->tail = tok;
}


/*
 * Definitions of implementation functions.
 */
struct token_node *_new_node() {
  struct token_node *list =
      (struct token_node *)calloc(1, sizeof(struct token_node));
  if (list == NULL)
    exit(-1);
  return list;
}

void _free_list(struct token_node *node) {
  if (node == NULL) {
    return;
  }
  _free_list(node->next);
  free(node);
  node = NULL;
}

void _for_each(struct token_node *list, void (*method)(const struct token_node *)) {
  for (struct token_node *ptr = list; ptr != NULL; ptr = ptr->next)
    method(ptr);
}

void _print(const struct token_node *tok) {
  const struct token *t = &tok->token;
  printf("{%ld,", t->priority);
  if(t->type != CNS) printf("%c}, ", t->id);
  else
  printf("%lf}, ", t->cnst);
}

