#include "elementary.h"

double b_add(double left, double right) { return left + right; }
double b_sub(double left, double right) { return left - right; }
double b_mult(double left, double right) { return left * right; }
double b_pow(double base, double exp) { return pow(base, exp); }
double b_log(double base, double arg) { return log(arg) / log(base); }
double b_div(double num, double den) { return num / den; }

double u_inv(double arg) { return 1.0 / arg; }
double u_nlog(double arg) { return log(arg); }
double u_cos(double arg) { return cos(arg); }
double u_sin(double arg) { return sin(arg); }
double u_tan(double arg) { return tan(arg); }
double u_sec(double arg) { return 1.0/cos(arg); }
double u_csc(double arg) { return 1.0/sin(arg); }
double u_cot(double arg) { return 1.0/tan(arg); }
double u_arccos(double arg) { return acos(arg); }
double u_arcsin(double arg) { return asin(arg); }
double u_arctan(double arg) { return atan(arg); }
double u_arcsec(double arg) { return acos(1.0/arg); }
double u_arccsc(double arg) { return asin(1.0/arg); }
double u_arccot(double arg) { return atan(1.0/arg); }



