#pragma once

#include "../objects/types.h"
#include "../objects/initializers.h"
#include "../objects/function.h"

#include <math.h>

int is_num(const vd_literal *check, double val, double tol);


int vb_add(vd_literal *o, const vd_literal *l, const vd_literal *r);

int vb_sub(vd_literal *o, const vd_literal *l, const vd_literal *r);

int vb_mul(vd_literal *o, const vd_literal *l, const vd_literal *r);

int vb_div(vd_literal *o, const vd_literal *l, const vd_literal *r);

int vb_pow(vd_literal *o, const vd_literal *l, const vd_literal *r);

int vb_log(vd_literal *o, const vd_literal *l, const vd_literal *r);

int vu_set(vd_literal *o, const vd_literal *i);

int vu_neg(vd_literal *o, const vd_literal *i);

int vu_sin(vd_literal *o, const vd_literal *i);

int vu_cos(vd_literal *o, const vd_literal *i);

int vu_tan(vd_literal *o, const vd_literal *i);

int vu_sec(vd_literal *o, const vd_literal *i);

int vu_csc(vd_literal *o, const vd_literal *i);

int vu_cot(vd_literal *o, const vd_literal *i);

int vu_asin(vd_literal *o, const vd_literal *i);

int vu_acos(vd_literal *o, const vd_literal *i);

int vu_atan(vd_literal *o, const vd_literal *i);

int vu_log(vd_literal *o, const vd_literal *i);
