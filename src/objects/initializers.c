#include "initializers.h"

Object as_mfcontext(context_modifier mod) {
  return (Object){.mContext = mod, .ty = CONTEXT};
}

void free_mfcontext([[maybe_unused]] mf_context *mod) { mod = NULL; }

Object as_bopliteral(b_opliteral opr) {
  return (Object){.bOperation = opr, .ty = BOPER};
}

void free_bopliteral([[maybe_unused]] b_opliteral *opr) { opr = NULL; }

Object as_uopliteral(u_opliteral opr) {
  return (Object){.uOperation = opr, .ty = UOPER};
}

void free_uopliteral([[maybe_unused]] u_opliteral *opr) { opr = NULL; }

void free_reader([[maybe_unused]] r_macro *mac) { mac = NULL; }
