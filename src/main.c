#include "interpret.h"
#include "environment/runtime.h"

int main(const int argc, const char *argv[]) {
  /*if (argc > 1)
    for (int argi = 1; argi < argc; argi++) {
      char location[1024];
      if (sscanf(argv[argi], "%1023s", location) == 0) {
        fprintf(stderr, "Invalid argument.");
        return -1;
      }
      read_file(location);
    }

  read_from_in();*/
  
  run_command_environment();

  return 0;
}
