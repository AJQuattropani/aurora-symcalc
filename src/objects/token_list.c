#include "token_list.h"

token_array new_token_array(size_t capacity) {
  token_array arr =
      (token_array){.data = (token *)malloc(capacity * sizeof(token)),
                    .capacity = capacity,
                    .size = 0};
  if (NULL == arr.data) {
    fprintf(stderr, "Malloc failed in %s", __func__);
    exit(1);
  }
  return arr;
}
void empty_token_array(token_array *arr) {
  memset(arr->data, 0, arr->capacity * sizeof(token));
  arr->size = 0;
}

void destroy_token_array(token_array *arr) {
  free(arr->data);
  arr->data = NULL;
  arr->capacity = 0;
  arr->size = 0;
}

void push_token_back(token_array *arr, const token *tok) {
  if (arr->size >= arr->capacity) {
    size_t new_cap = 2 * arr->size;
    token *temp = (token *)realloc(arr->data, new_cap * sizeof(token));
    if (NULL == temp) {
      fprintf(stderr, "Realloc failed in %s", __func__);
      exit(1);
    }
    arr->data = temp;
    arr->capacity = new_cap;
  }
  arr->data[arr->size] = *tok;
  arr->size++;
}

