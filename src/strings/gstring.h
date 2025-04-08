#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mstring.h"

/**
  * Growing String: a dynamic string type for strings that
  * tend to grow.
  */
struct growing_string;
typedef struct growing_string gString;
struct growing_string {
  char* cstring;
  size_t capacity;
  size_t size;
};


gString g_from_cstr(const char *cstr);

gString g_from_capacity(size_t capacity);

gString g_from_copy(const gString *gstr);

void g_deletestr(gString *gstr);

void g_empty(gString *gstr);

void g_append_back(gString *string, const char *appendix, size_t len);




