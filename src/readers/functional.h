#pragma once

#include "function_reader.h"



/*
* Handles various command types associated with function token.
*/
void function_command(env *context, f_object *fun, const token_array *args);


/*
* Implementation of function evaluation for general use.
*/
void evaluate_function_imp(f_node *fun, vd_literal *out, const vd_literal *in);


/*
* Simplify the function.
*/
void simplify_command(env *context, const token_array *args);

/*
* Implementation of simplication procedure.
*/
void simplify_imp(f_node *fun);

