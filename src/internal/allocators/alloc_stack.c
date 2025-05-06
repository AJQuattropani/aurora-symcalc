#include "alloc_stack.h"

void *s_alloc(stack *stack, size_t n) {
  // fprintf(stdout, "Requested %ld bytes from buffer of size %ld.\n", n,
  // stack->size);
  void *ret = stack->buff + stack->top;
  if (stack->top + n > stack->size) {
    fprintf(stderr,
            "[FATAL] insufficient memory for salloc request at %p + %ld, of "
            "%ld bytes.\n",
            (void *)stack->buff, stack->top, n);
    exit(1);
  }
  stack->top += n;
  return ret;
}

void free_stack(stack *stack) {
  free(stack->buff);
  memset(stack, 0, sizeof(struct stack));
}

stack new_stack(size_t num_bytes) {
  stack stack = {
      .buff = (BYTE *)(malloc(num_bytes)), .size = num_bytes, .top = 0};
  if (NULL == stack.buff) {
    fprintf(stderr, "[FATAL] malloc failed in %s\n", __func__);
    exit(1);
  }
  return stack;
}
