#include "terminal.h"
#ifdef TERMINAL
#endif

#include "env/environment.h"

int main(const int argc, char *argv[]) { 
  {
    env env;
    init_env(&env, argc, argv);
    runtime(&env);
    free_env(&env);
  }
  printf("Environment terminated.\n");
  return 0;
}


