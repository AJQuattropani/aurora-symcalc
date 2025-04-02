#pragma once

#include <stdlib.h>
#include "../functional/function.h"

struct environment;

typedef void(*reserved)(struct environment* state, char **args, const size_t argn);


