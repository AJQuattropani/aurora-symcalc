#pragma once

#include "../internal/token_list.h"
#include "../objects/vector.h"

/*
 * Instructs to read remaining tokens as a vector.
 */
void read_vector(Object *o, token_array *args);

/*
 * Instructs to read remaining tokens as arguments for defining a linear space.
 */
void read_linspace(Object *o, token_array *args);

/*
 * Instructs to read remaining tokens as arguments for defining a counting
 * space.
 */
void read_countspace(Object *o, token_array *args);

/*
 * Instructs to read remaining token as a scalar.
 */
void read_scalar(Object *o, token_array *args);

/*
 * Generic implementation of reading a scalar for use anywhere.
 */
Object read_scalar_imp(const token *arg);

/*
 * Generic function for attempting to read double.
 * Returns 0 on success, 1 on failure.
 */
int try_read_double(double *target, const mString *mstr);
