#pragma once

#include <stdio.h>

#include "../internal/allocators/gstring.h"
#include "../internal/types.h"
#include "../internal/allocators/alloc_stack.h"

Object as_vdliteral(vd_literal *value);

Object as_vdliteral_mv(vd_literal value);

vd_literal make_scalar(double value);

void free_vdliteral(vd_literal *lit);

vd_literal alloc_vdliteral(vector_size_t n);

vd_literal copy_vdliteral(const vd_literal *other);

void sprint_vector(gString *inp, const vd_literal *value);

typedef struct {
  size_t size;
  vd_literal *data;
} vector_list;

vector_list alloc_vdlist(size_t size, vector_size_t ncomponents);

void free_vdlist(vector_list *list);
