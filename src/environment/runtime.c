#include "runtime.h"

void run_command_environment() {
  // main loop
  struct environment env = initialize_environment();

  read_with_state(&env);

  empty_symbolmap(&env.symbol_map); // clear symbolmap
  
  printf("\nFinished running program.\n");
}
