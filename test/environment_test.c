#include "../src/env/environment.h"

int main(int argc, char *argv[]) {
  {
    env env;
    init_env(&env, argc, argv);
    runtime(&env);
    free_env(&env);
  }
  return 0;
}


