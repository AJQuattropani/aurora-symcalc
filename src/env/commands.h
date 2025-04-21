#pragma once

#include "environment.h"
#include "script_stack.h"

#include "../objects/function.h"
#include "../objects/hash_map.h"
#include "../objects/token_list.h"

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
 * Command for adding new files to the stack.
 */
void open_files(env *context, const token_array *args);

/*
* Instructs to read remaining tokens as a vector.
*/
Object read_vector(const token_array *args);

/*
* Instructs to read remaining tokens as a function.
*/
Object read_function(const token_array *args);


