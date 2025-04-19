#pragma once

#include "types.h"
#include "../strings/mstring.h"
#include "hash_map.h"

struct token {
  _kvpair* token;
  //mString key;
  //Object token;
  int priority;
};

struct token_array {
  token *data;
  size_t capacity;
  size_t size;
};

token_array new_token_array(size_t capacity);

void empty_token_array(token_array *arr);

void destroy_token_array(token_array *arr);

void push_token_back(token_array *arr, const token *tok);
