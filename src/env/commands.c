#include "commands.h"

void exit_env(env *context, [[maybe_unused]] const token_array *args) {
  context->status = EXIT;
}

void return_env(env *context, [[maybe_unused]] const token_array *args) {
  context->status = RETURN;
}

void reset_env(env *context, [[maybe_unused]] const token_array *args) {
  empty_map(context->map);
  default_map(context->map);
  context->status = OK;
}

void print_env(env *context, [[maybe_unused]] const token_array *args) {
  print_map(context->map);
}

void print_tok(env *context, const token_array *args) {
  if (2 > args->size) {
    fprintf(stderr, "[SKIPPED] Insufficient arguments for print call.\n");
    return;
  }

  g_append_back_c(&context->output_buffer, "Peak: ");
  for (size_t i = 1; i < args->size; i++) {
    token *curr = &args->data[i];
    g_append_back_c(&context->output_buffer, curr->token->key.cstring);
    g_append_back_c(&context->output_buffer, " ");
    sprint_object(&context->output_buffer, &curr->token->value);
    g_append_back_c(&context->output_buffer, " ");
  }
}

void define_object([[maybe_unused]] env *context, const token_array *args) {
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
  if (READER != read_type->ty) {
    fprintf(stderr, "[SKIPPED] Invalid type.\n");
    return;
  }
  r_macro read_macro = read_type->reader;
  *into = read_macro(&(const token_array){
      .data = args->data + 3,
      .capacity = 0,
      .size = args->size -
              3}); // read in remaining args as either function or vector
}

void open_files(env *context, const token_array *args) {
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

Object read_function(const token_array *args) {
  unsigned short argnum = 0;
  for (; argnum < args->size; argnum++) {
    if (SYNTAX_RIGHT == args->data[argnum].token->value.ty)
      break;
    if (NONE != args->data[argnum].token->value.ty) {
      fprintf(stderr, "[ERROR] Function argument collision at %s\n",
              args->data[argnum].token->key.cstring);
      return (Object){{{0}}, .ty = NONE};
    }
    args->data[argnum].token->value = (Object){
        .ty = TEMP, .other = (long long int)argnum, .priority = USHRT_MAX};
  }

  unsigned short offs = argnum + 1;
  if (offs >= args->size) {
    fprintf(stderr, "[ERROR] Please provide a function body.\n");
    return (Object){{{0}}, .ty = NONE};
  }

  // prepass to avoid an obvious source of recursive deletion hell
  for (size_t i = offs; i < args->size; i++) {
    if (args->data[i].token->value.ty == UOPER ||
        args->data[i].token->value.ty == BOPER ||
        args->data[i].token->value.ty == TEMP ||
        args->data[i].token->value.ty == FUNC ||
        args->data[i].token->value.ty == VECTOR ||
        args->data[i].token->value.ty == NONE)
      continue;
    fprintf(stderr, "[ERROR] Invalid token %s passed to %s.\n",
            args->data[i].token->key.cstring, __func__);
    return (Object){{{0}}, .ty = NONE};
  }

  f_node *root = least_significant_token_recurse(
      &(token_array){.data = args->data + offs, .size = args->size - offs});
  if (NULL == root) {
    return (Object){{{0}}, .ty = NONE};
  }

  f_object function = {.root = root, .argcnt = argnum};
  return as_fobject(&function);
}
