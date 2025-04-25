#pragma once

#include "../objects/function.h"
#include "../objects/token_list.h"
#include "../env/commands.h"

/*
* Instructs to read remaining tokens as a function.
*/
void read_function(Object *obj, const token_array *args);

