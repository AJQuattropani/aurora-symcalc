#include "commands.h"

void exit_env(env *context, [[maybe_unused]] const token_array *args) {
  context->status = EXIT;
}

void reset_env(env *context, [[maybe_unused]] const token_array *args) {
  empty_map(&context->map);
  default_map(&context->map);
  context->status = OK;
}

//void open_file(env *context, [[maybe_unused]] vList *args) {
  

//}

