#include "commands.h"

void exit_env(env *context, vList *args) {
  empty_map(&context->map);
  
  context->current_file = NULL;
  context->status = EXIT;
}
void reset_env(env *context, vList *args) {
  empty_map(&context->map);
  default_map(&context->map);

  context->status = OK;
}

void open_file(env *context, vList *args) {
  


}

