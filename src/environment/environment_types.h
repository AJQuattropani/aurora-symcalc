#pragma once

#include <stdlib.h>

struct environment;

typedef void(*reserved)(struct environment* state, char **args, const size_t argn);


