#pragma once

#include "../objects/function.h"
#include "../internal/token_list.h"
#include "../internal/allocators/alloc_stack.h"
#include "../objects/packed_function.h"

/*
* Instructs to read remaining tokens as a function.
*/
void read_function(Object *obj, token_array *args);

void read_copy_packed(Object *obj, token_array *args);



