#include "commands.h"

void exit_env(env *restrict context, [[maybe_unused]] const token_array *restrict args) {
  context->status = EXIT;
}

void return_env(env *restrict context, [[maybe_unused]] const token_array *restrict args) {
  context->status = RETURN;
}

void reset_env(env *restrict context, [[maybe_unused]] const token_array *restrict args) {
  empty_map(context->map);
  default_map(context->map);
  context->status = OK;
}

void print_env(env *restrict context, [[maybe_unused]] const token_array *restrict args) {
  print_map(context->map);
}

void print_tok(env *restrict context, const token_array *restrict args) {
  g_append_back_c(&context->output_buffer, "\n\t");
  if (2 > args->size) {
    fprintf(stderr, "[SKIPPED] Insufficient arguments for print call.\n");
    return;
  }

  for (size_t i = 1; i < args->size; i++) {
    token *curr = &args->data[i];
    g_append_back_c(&context->output_buffer, curr->token->key.cstring);
    g_append_back_c(&context->output_buffer, " ");
    sprint_object(&context->output_buffer, &curr->token->value);
    g_append_back_c(&context->output_buffer, ",");
  }
}

void define_object([[maybe_unused]] env *restrict context, const token_array *restrict args) {
  if (4 > args->size) {
    fprintf(stderr, "[SKIPPED] Insufficient arguments for set call.\n");
    return;
  }
  Object *into = &args->data[1].token->value;
  if (NONE != into->ty) {
    _key *str = &args->data[1].token->key;
    fprintf(stderr,
            "[SKIPPED] Writing to existing type \"%.*s\" not supported.\n",
            (int)str->size, str->cstring);
    return;
  }

  Object *read_type = &args->data[2].token->value;
  if (READER != read_type->ty && SYNTAX_EQUALS != read_type->ty) {
    fprintf(stderr, "[SKIPPED] Invalid type %s.\n",
            args->data[2].token->key.cstring);
    return;
  }
  r_macro read_macro = read_type->reader;
  read_macro(
      into,
      &(token_array){
          .data = args->data + 3,
          .capacity = 0,
          .size = args->size -
                  3}); // read in remaining args as either function or vector
}

void delete_object(env * restrict context, const token_array * restrict args) {
  static _mnode empty = {.value = {.ty = NONE}};
  if (2 > args->size) {
    fprintf(stderr, "[SKIPPED] No variables listed.\n");
  }
  for (size_t i = 1; i < args->size; i++) {
    for (size_t j = i + 1; j < args->size; j++) {
      if (args->data[i].token == args->data[j].token) {
        args->data[j].token = &empty;
      }
    }
  }

  for (size_t i = 1; i < args->size; i++) {
    _mnode *fig = args->data[i].token;
    obj_t ty = fig->value.ty;
    switch (ty) {
    case NONE:
      __attribute__((fallthrough));
    case TEMP:
      __attribute__((fallthrough));
    case SYNTAX_EQUALS:
      __attribute__((fallthrough));
    case CONTEXT:
      __attribute__((fallthrough));
    case READER:
      __attribute__((fallthrough));
    case BOPER:
      __attribute__((fallthrough));
    case UOPER:
      g_append_back_c(&context->output_buffer,
                      "\n| [SKIPPED] Token is not a deletable type.");
      continue;
    case VECTOR:
      __attribute__((fallthrough));
    case FUNC:
      __attribute__((fallthrough));
    case PFUNC:
      g_append_back_c(&context->output_buffer, "\n| Token removed.");
      remove_node(fig);
      continue;
    }
    __UNREACHABLE_BRANCH
  }
}

void open_files(env *restrict context, const token_array *restrict args) {
  if (1 > args->size) {
    fprintf(stderr, "[SKIPPED] Insufficient arguments for open call.\n");
    return;
  }
  for (size_t i = args->size - 1; i > 0; i--) {
    token *node = &args->data[i];
    if (0 == open_file(&context->script_stack, node->token->key.cstring)) {
      context->status = CALL;
    }
  }
}
