#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct immutable_string;
typedef struct immutable_string mString;
struct immutable_string {
  char* string;
  size_t size;
};

mString from_cstr(const char *cstr);

mString from_size(size_t strlen);

mString from_copy(mString mstr);

void delete_mstr(mString *mstr);

int msame(const mString *str1, const mString *str2);
