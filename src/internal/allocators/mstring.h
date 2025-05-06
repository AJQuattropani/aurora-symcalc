#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Immutable String: a barebones string type for hashing, copying,
 * and comparison.
 */

struct immutable_string;
typedef struct immutable_string mString;
struct immutable_string {
  char *cstring;
  size_t size;
};

mString m_from_cstr(const char *cstr);

mString m_wrapper(char *cstr);

mString m_from_size(size_t strlen);

mString m_from_copy(mString mstr);

void m_deletestr(mString *mstr);

int m_same(const mString *str1, const mString *str2);
