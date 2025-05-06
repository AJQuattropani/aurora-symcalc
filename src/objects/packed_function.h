#pragma once

#include "../internal/allocators/alloc_stack.h"
#include "function.h"

pf_object make_packed_copy(f_object *object);

void free_pobject(pf_object *object);
