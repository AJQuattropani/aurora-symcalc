#pragma once
#include "../objects/types.h"
#include "../objects/hash_map.h"

enum env_status {
  OK,
  EXIT
};
typedef enum env_status env_status;

struct environment {
  Map map;
  FILE* current_file;
  env_status status;
};

