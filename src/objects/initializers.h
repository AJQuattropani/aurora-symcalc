#pragma once

#include "types.h"
#include "function.h"
#include "vector.h"

Object as_mfcontext(context_modifier mod);

void free_mfcontext(mf_context *mod);

Object as_bopliteral(b_opliteral opr);

void free_bopliteral(b_opliteral *opr);

Object as_uopliteral(u_opliteral opr);

void free_uopliteral(u_opliteral *opr);

void free_reader(r_macro *mac);
