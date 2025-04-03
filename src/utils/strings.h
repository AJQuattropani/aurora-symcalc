#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct string {
  size_t size;
  size_t capacity;
  char* data;
};

struct string new_string(size_t capacity);

void free_string(struct string *string);

void empty_string(struct string *string);

void append_back(struct string *string, const char *appendix, size_t len);
