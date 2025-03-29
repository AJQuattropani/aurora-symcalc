#include "interpret.h"

int main(int argc, char *argv[]) {
  if (argc > 2) {
    printf("Running %10s\n", argv[0]);
    char fun[256];
    double x;
    if (sscanf(argv[1], "%255s", &fun[0]) == 0) return -5;
    if (sscanf(argv[2], "%lf", &x) == 0) return -5;
    struct Operation *oper = build_function(&fun[0], strlen(fun));
    printf("%lf\n", a_evaluate(oper, x));
    a_free_recursive(oper);
    return 0;
  }
  return 4;
}
