#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mstring.h"

/**
 * Growing String: a dynamic string type for strings that
 * tend to grow.
 */
struct growing_string;
typedef struct growing_string gString;
struct growing_string {
  char *cstring;
  size_t capacity;
  size_t size;
};

gString g_from_capacity(size_t capacity);

void g_deletestr(gString *gstr);

void g_empty(gString *gstr);

void g_append_back(gString *gstr, const char *appendix, size_t len);

void g_append_back_c(gString *gstr, const char *appendix);
