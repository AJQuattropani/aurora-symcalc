#pragma once

#include "../objects/vector.h"
#include "../objects/token_list.h"

/*
* Instructs to read remaining tokens as a vector.
*/
void read_vector(Object *o, const token_array *args);

/*
 * Instructs to read remaining token as a scalar.
 */
void read_scalar(Object *o, const token_array *args);

/*
 * Generic implementation of reading a scalar for use anywhere.
 */
Object read_scalar_imp(const token *arg);

/*
* Generic function for attempting to read double.
* Returns 0 on success, 1 on failure.
*/
int try_read_double(double* target, const mString *mstr);
