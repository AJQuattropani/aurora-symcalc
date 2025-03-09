#pragma once

#include <stdio.h>
#include <math.h>

/**
 * Repeated addition of args.
  */
double a_add(const double *args, size_t num_subops);

/**
  * Repeated multiplication of args.
  */
double a_mult(const double *args, size_t num_subops);

/**
  * Nested powers of args.
  */
double a_pow(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_sin(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_cos(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations. 
  */
double a_tan(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_sec(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_csc(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_cot(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_arcsin(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_arccos(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_arctan(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_arccot(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_arcsec(const double *args, size_t num_subops);

/**
  * Repeated mulitiplication of trig evaluations.
  */
double a_arccsc(const double *args, size_t num_subops);

/**
  * Nested change-of-base of logarithm.
  */
double a_log(const double *args, size_t num_subops);



