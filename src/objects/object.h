#pragma once
#include <string.h>

#include "../internal/allocators/gstring.h"

#include "function.h"
#include "initializers.h"
#include "packed_function.h"
#include "vector.h"

Object null_object();

void sprint_object(gString *gstr, const Object *o);

void free_object(Object *o);
