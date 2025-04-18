#include "commands.h"

void exit_env(env *context, [[maybe_unused]] vList *args) {
  context->status = EXIT;
  printf("EXIT!\n");
}

void reset_env(env *context, [[maybe_unused]] vList *args) {
  empty_map(&context->map);
  default_map(&context->map);
  context->status = OK;
  printf("RESET!\n");
}

void open_file([[maybe_unused]] env *context, [[maybe_unused]] vList *args) {
  printf("OPEN!\n");


}

