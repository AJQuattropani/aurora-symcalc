#include "initializers.h"

Object as_mfcontext(context_modifier mod) {
  return (Object){.mContext = mod, .ty = CONTEXT};
}

void free_mfcontext(mf_context *mod) { mod = NULL; }

Object as_bopliteral(b_opliteral opr) {
  return (Object){.bOperation = opr, .ty = BOPER};
}

void free_bopliteral(b_opliteral *opr) { opr = NULL; }

Object as_uopliteral(u_opliteral opr) {
  return (Object){.uOperation = opr, .ty = UOPER};
}

void free_uopliteral(u_opliteral *opr) { opr = NULL; }


