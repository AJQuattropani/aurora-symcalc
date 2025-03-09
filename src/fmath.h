#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
* Default value for an operation, outputs first variable. 
*/
double _identity(const double a, const double b);

double _add(const double a, const double b);

double _mult(const double a, const double b);

/**
* a to the power of b.
*/
double _pow(const double a, const double b);
