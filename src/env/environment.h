#pragma once

#include "unistd.h"

#include "../objects/types.h"
#include "../objects/hash_map.h"

#include "../strings/gstring.h"

#include "commands.h"
#include "operators.h"
#include "interpret.h"

enum env_status {
  OK,
  EXIT
};
typedef enum env_status env_status;

struct environment {
  Map map;
  FILE* current_file;
  env_status status;
  gString output_buffer;
};

void default_map(Map map);

void init_env(env *env);

void free_env(env *env);

void runtime();
