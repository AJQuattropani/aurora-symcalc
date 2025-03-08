#include <stdio.h>

#include "function.h"

int main(int argc, char *argv[]) {
  //printf("Identity address: %p | Add address : %p | Mult address: %p \n",
  //       &identity, &add, &mult);

  if (argc < 2)
    return -1;

  printf("Building function with %d steps.\n", argc - 1);
  struct Function func = build_function(argc - 1, argv + 1);

  print(&func);

  double x = 0;
  char buffer[64];

  while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    if (buffer[0] == '\n' || buffer[0] == '\0') break;
    sscanf(buffer, "%lf", &x);
    double y = evaluate(&func, x);
    printf("Output: %lf\n", y);
  }

  destroy(&func);

  return 0;
}
