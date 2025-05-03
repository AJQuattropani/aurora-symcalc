#pragma once

#include "environment.h"
#include "script_stack.h"

#include "../objects/function.h"

#include "../internal/hash_map.h"
#include "../internal/token_list.h"

#include "../readers/vector_reader.h"
#include "../readers/function_reader.h"
#include "../readers/functional.h"

/*
 * Applicable functions for CONTEXT type
 */

/*
* FLAGS ENVIRONMENT TO EXIT.
*/
void exit_env(env *context, const token_array *args);

/*
* FLAGS ENVIRONMENT TO RETURN.
*/
void return_env(env *context, const token_array *args);


/*
* FLAGS ENVIRONMENT TO RESET.
*/
void reset_env(env *context, const token_array *args);

/*
 * PRINTS ALL ENVIRONMENT VARIABLES.
 */
void print_env(env *context, [[maybe_unused]] const token_array *args);

/*
 *  PRINTS LIST OF DESIRED TOKENS.
 */
void print_tok(env *context, const token_array *args);

/*
* Makes a new object.
* arg 0 : set
* arg 1 : name
* arg 2 : method to read (vector, function, etc.)
* arg 3 : values to read in
*/
void define_object(env *context, const token_array *args);

/*
 * Deletes the token.
 * arg 0: delete
 * arg 1: name
 */
void delete_object(env *context, const token_array *args);

/*
 * Command for adding new files to the stack.
 */
void open_files(env *context, const token_array *args);


