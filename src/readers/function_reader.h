#pragma once

#include "../objects/function.h"
#include "../objects/token_list.h"
#include "../env/commands.h"

/*
* Instructs to read remaining tokens as a function.
*/
Object read_function(const token_array *args);


void function_command(env *context, const token_array *args);
void evaluate_function_imp(f_node *fun, const vd_literal *in);


