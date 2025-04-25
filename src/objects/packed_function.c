#include "packed_function.h"

byte *salloc(stack_arena *arr, size_t num_bytes) {
  byte *ret = arr->buffer + arr->byte_ptr;
  arr->byte_ptr += num_bytes;
  if (arr->byte_ptr >= arr->size) {
    fprintf(stderr, "[FATAL] %s\n call overflowed.", __func__);
    exit(2);
  }
  return ret;
}

void free_stack_arena(stack_arena *arena) {
  free(arena->buffer);
  *arena = (stack_arena){0};
}

stack_arena new_stack_arena(size_t num_bytes) {
  byte *buffer = (byte *)malloc(num_bytes);
  if (NULL == buffer) {
    fprintf(stderr, "[FATAL] malloc failed in %s\n", __func__);
    exit(1);
  }
  return (stack_arena){.byte_ptr = 0, .size = num_bytes, .buffer = buffer};
}

