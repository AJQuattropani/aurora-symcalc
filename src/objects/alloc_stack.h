#pragma once

#include <stdlib.h>

#include "types.h"

struct stack;
typedef struct stack stack;
struct stack {
  BYTE* buff;
  size_t size;
  size_t top;
};

stack new_stack(size_t num_bytes);

void free_stack(stack *stack);

void *s_alloc(stack *stack, size_t n);
