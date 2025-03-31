#include "string_list.h"

struct stringview_list new_list(size_t n_capacity) {
  struct stringview_list new_list = {
      .data = (char **)malloc(n_capacity * sizeof(char *)),
      .capacity = n_capacity,
      .size = 0};
  if (NULL == new_list.data) {
    fprintf(stderr, "Mallocation in new_list failed.");
    exit(-1);
  }
  return new_list;
}

void push_view(struct stringview_list *list, char *reference) {
  if (list->size >= list->capacity) {
    size_t new_capacity = list->size * 2;
    char **temp = (char **)realloc(list->data, new_capacity * sizeof(char *));
    if (NULL == temp) {
      fprintf(stderr, "Reallocation in push_view failed. Skipping push.");
      return;
    }
    list->data = temp;
    list->capacity = new_capacity;
  }
  list->data[list->size] = reference;
  list->size += 1;
}

void empty_stringlist(struct stringview_list *list) {
  if (NULL == list->data) {
    fprintf(stderr, "Skipping attempt to write to freed buffer.");
    return;
  }
  memset(list->data, 0, list->capacity * sizeof(char *));
  list->size = 0;
}

void free_stringlist(struct stringview_list *list) {
  list->capacity = 0;
  list->size = 0;
  if (NULL == list->data) {
    fprintf(stderr, "Skipping attempt to free list.");
  }
  free(list->data);
  list->data = NULL;
}

