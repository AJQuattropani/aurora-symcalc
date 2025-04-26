#pragma once
#include <string.h>

#include "../strings/gstring.h"

#include "initializers.h"
#include "vector.h"
#include "function.h"
#include "packed_function.h"

Object null_object();

void sprint_object(gString *gstr, const Object *o);

void free_object(Object *o);
