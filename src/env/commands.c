#include "commands.h"

void exit_env(env *context, [[maybe_unused]] const token_array *args) {
  context->status = EXIT;
}

void reset_env(env *context, [[maybe_unused]] const token_array *args) {
  empty_map(&context->map);
  default_map(&context->map);
  context->status = OK;
}

void print_env(env *context, [[maybe_unused]] const token_array *args) {
  print_map(&context->map);
}

void define_object([[maybe_unused]] env *context, const token_array *args) {
  if (4 > args->size) {
    fprintf(stderr, "[SKIPPED] Insufficient arguments for set call.\n");
    goto catch_error;
  }
  Object *into = &args->data[1].token->value;
  if (NONE != into->ty) {
    _key *str = &args->data[1].token->key;
    fprintf(stderr, "[SKIPPED] Writing to existing type \"%.*s\" not supported.\n", (int)str->size, str->cstring);
    goto catch_error;
  }

  Object *read_type = &args->data[2].token->value;
  if (READER != read_type->ty) {
    fprintf(stderr, "[SKIPPED] Invalid type.\n");
    goto catch_error;
  }
  r_macro read_macro = read_type->reader;
  *into = read_macro(&(const token_array){
      .data = args->data + 3,
      .capacity = 0,
      .size = args->size -
              3}); // read in remaining args as either function or vector

catch_error:
  for (size_t i = 0; i < args->size; i++) {
    token *curr = &args->data[i];
    if (NONE == curr->token->value.ty)
      remove_node(curr->token);
  } // clean up newly-hashed nodes
}

Object read_vector(const token_array *args) {
  vd_literal vector = alloc_vdliteral(args->size);
  for (size_t i = 0; i < args->size; i++) {
    token *curr = &args->data[i];
    if (0 >= sscanf(curr->token->key.cstring, "%lf", &vector.data[i])) {
      fprintf(stderr,
              "[SKIPPED] Non-double definition provided for input %ld.\n", i);
      vector.data[i] = 0;
      continue;
    }
  }
  return as_vdliteral(&vector);
}

// void open_file(env *context, [[maybe_unused]] vList *args) {
// }
