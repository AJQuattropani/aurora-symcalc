#pragma once

#include "environment.h"

#include "../objects/function.h"

#include "../internal/allocators/script_stack.h"
#include "../internal/hash_map.h"
#include "../internal/token_list.h"
#include "../internal/procedures/representations.h"

#include "../readers/function_reader.h"
#include "../readers/functional.h"
#include "../readers/vector_reader.h"


/*
 * Applicable functions for CONTEXT type
 */

void exit_env(env *context, const token_array *args);

void return_env(env *context, const token_array *args);

void reset_env(env *context, const token_array *args);

void print_env(env *context, [[maybe_unused]] const token_array *args);

void print_tok(env *context, const token_array *args);

void define_object(env *context, const token_array *args);

void delete_object(env *context, const token_array *args);

void open_files(env *context, const token_array *args);

void print_tree(env *context, const token_array *args);

void set_output(env *context, const token_array *args);
