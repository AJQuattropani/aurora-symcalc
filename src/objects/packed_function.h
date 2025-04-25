#pragma once

#include "function.h"

typedef uint8_t byte;
typedef struct {
  size_t byte_ptr;
  size_t size;
  byte* buffer;
} stack_arena;

stack_arena new_stack_arena(size_t num_bytes);

void free_stack_arena(stack_arena *arena);

byte *salloc(stack_arena *arr, size_t num_bytes);





