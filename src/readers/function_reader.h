#pragma once

#include "../objects/function.h"
#include "../objects/function_stack.h"
#include "../objects/token_list.h"
#include "../objects/packed_function.h"
#include "../env/commands.h"

/*
* Instructs to read remaining tokens as a function.
*/
void read_function(Object *obj, token_array *args);

void read_copy_packed(Object *obj, token_array *args);



