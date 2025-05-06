#include "packed_function.h"

#include <stddef.h>

void count_bytes(size_t *counter, f_node *node) {
  *counter += offsetof(f_node, __align_data);
  *counter += node->name.size;
  switch (node->ty) {
  case BINARY:
    *counter += sizeof(struct binary_f);
    // fprintf(stdout, "Request: counter %ld\n", *counter);
    count_bytes(counter, node->bf.left);
    count_bytes(counter, node->bf.right);
    return;
  case UNARY:
    *counter += sizeof(struct unary_f);
    // fprintf(stdout, "Request: counter %ld\n", *counter);
    count_bytes(counter, node->uf.in);
    return;
  case CONSTANT:
    *counter += sizeof(struct const_f);
    // fprintf(stdout, "Request: counter %ld\n", *counter);
    return;
  case IDENTITY:
    *counter += sizeof(struct iden_f);
    // fprintf(stdout, "Request: counter %ld\n", *counter);
    return;
  }
}

f_node *f_pack_duplicate(stack *stack, const f_node *ref) {
  f_node *dup;
  switch (ref->ty) {
  case BINARY:
    dup = (f_node *)s_alloc(stack, offsetof(f_node, __align_data) +
                                       sizeof(struct binary_f));
    dup->bf.op = ref->bf.op;
    dup->bf.left = f_pack_duplicate(stack, ref->bf.left);
    dup->bf.right = f_pack_duplicate(stack, ref->bf.right);
    break;
  case UNARY:
    dup = (f_node *)s_alloc(stack, offsetof(f_node, __align_data) +
                                       sizeof(struct unary_f));
    dup->uf.op = ref->uf.op;
    dup->uf.in = f_pack_duplicate(stack, ref->uf.in);
    break;
  case CONSTANT:
    dup = (f_node *)s_alloc(stack, offsetof(f_node, __align_data) +
                                       sizeof(struct const_f));
    dup->cf.output = ref->cf.output;
    break;
  case IDENTITY:
    dup = (f_node *)s_alloc(stack, offsetof(f_node, __align_data) +
                                       sizeof(struct iden_f));
    dup->xf.index = ref->xf.index;
    break;
  }
  dup->ty = ref->ty;
  dup->priority = ref->priority;
  dup->depth_index = ref->depth_index;
  {
    char *ncstr = (char *)s_alloc(stack, ref->name.size);
    strncpy(ncstr, ref->name.cstring, ref->name.size);
    dup->name = (mString){.cstring = ncstr, .size = ref->name.size};
  }
  return dup;
}

pf_object make_packed_copy(f_object *fobj) {
  size_t num_bytes = 0;
  num_bytes += sizeof(f_object);

  count_bytes(&num_bytes, fobj->root);
  // fprintf(stdout, "%ld bytes counted.\n", num_bytes);
  stack function_stack = new_stack(num_bytes);
  f_object *function_head =
      (f_object *)s_alloc(&function_stack, sizeof(f_object));
  function_head->attr = fobj->attr;
  function_head->root = f_pack_duplicate(&function_stack, fobj->root);

  return (pf_object){.fObj = function_head};
}

void free_pobject(pf_object *object) { free(object->fObj); }
