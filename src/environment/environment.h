#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symbol_map.h"

enum environment_status {
  OK = 0,
  ESC,
  PANIC
};


struct environment {
  enum environment_status status;
  struct symbol_map symbol_map;
  FILE* current_file;
};



void execute_from_symbol(struct environment *state,
                         const struct symbol *symbol, char **args, const size_t argn);

// methods of the environment

struct environment initialize_environment();
