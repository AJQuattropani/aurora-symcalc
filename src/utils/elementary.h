#pragma once

#include <math.h>
#include <float.h>

#include "operation.h"

// Binary functions
double b_add(double left, double right);
double b_sub(double left, double right);
double b_mult(double left, double right);
double b_pow(double base, double exp);
double b_log(double arg, double base);
double b_div(double num, double den);

// Unary functions
double u_inv(double arg);
double u_nlog(double arg);
double u_cos(double arg);
double u_sin(double arg);
double u_tan(double arg);
double u_sec(double arg);
double u_csc(double arg);
double u_cot(double arg);
double u_arccos(double arg);
double u_arcsin(double arg);
double u_arctan(double arg);
double u_arcsec(double arg);
double u_arccsc(double arg);
double u_arccot(double arg);
