#pragma once

#include "types.h"

struct token;
typedef struct token token;
struct token {
  Object* token;
  int priority;
};

struct token_array;
typedef struct token_array token_array;
struct token_array {
  token *data;
  size_t capacity;
  size_t size;
};

token_array new_array(size_t capacity);

void empty_array(token_array *arr);

void destroy_array(token_array *arr);

void push_back(token_array *arr, const token *tok);
