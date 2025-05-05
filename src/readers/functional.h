#pragma once

#include "function_reader.h"
#include "../env/environment.h"

/*
 * Writes output of function into object buffer.
 */
void read_eval(Object *obj, token_array *args);

/*
 * Returns a function if its just the first token. Returns evaluation provided arguments.
 */
Object function_eval(f_object *fun, const token_array *args);

/*
 * Handles various command types associated with function token.
 */
void function_command(env *context, f_object *fun, const token_array *args);

/*
 * Implementation of function evaluation for general use.
 */
vd_literal output_eval(size_t index, f_node *root, depth_t depth, vd_literal *inp_args, vector_size_t inp_size);


