#pragma once
#include <string.h>

#include "../strings/gstring.h"

#include "initializers.h"
#include "vector.h"
#include "function.h"

void sprint_object(gString *gstr, const Object *o);

void free_object(Object *o);
