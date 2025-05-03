#pragma once

#include "function.h"
#include "../internal/alloc_stack.h"

pf_object make_packed_copy(f_object *object);

void free_pobject(pf_object *object);
