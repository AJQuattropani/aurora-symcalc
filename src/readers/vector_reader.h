#pragma once

#include "../objects/vector.h"
#include "../objects/token_list.h"

/*
* Instructs to read remaining tokens as a vector.
*/
Object read_vector(const token_array *args);

/*
* Generic function for attempting to read double.
* Returns 0 on success, 1 on failure.
*/
int try_read_double(double* target, const mString *mstr);
