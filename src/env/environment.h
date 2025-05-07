#pragma once

#include "unistd.h"

#include "../internal/hash_map.h"
#include "../internal/types.h"

#include "../internal/procedures/differentiate.h"
#include "../internal/procedures/operators.h"
#include "../internal/procedures/simplify.h"

#include "../internal/allocators/gstring.h"

#include "../internal/allocators/script_stack.h"
#include "commands.h"
#include "interpret.h"

enum env_status { OK, CALL, RETURN, EXIT };
typedef enum env_status env_status;

struct environment {
  Map map;
  sc_stack script_stack;
  env_status status;
  gString output_buffer;
};

void default_map(Map map);

void init_env(env *env, int argc, char **argv);
void runtime(env *env);
void free_env(env *env);
