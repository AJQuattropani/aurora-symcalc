#pragma once

#include <assert.h>

#include "operators.h"
#include "../token_list.h"
#include "../../objects/function.h"
#include "../../readers/functional.h"

static __attribute__((always_inline)) inline void
update_depth_max(depth_t depth, depth_t *depth_max) {
  *depth_max = (depth ^ ((depth ^ *depth_max) & -(depth < *depth_max))) + 1;
}

f_node *diff_bin(const f_node *in, vector_size_t argidx, f_attribs *attr);

f_node *diff_un(const f_node *in, vector_size_t argidx, f_attribs *attr);

f_node *diff_cnst(const f_node *in, vector_size_t argidx, f_attribs *attr);

f_node *diff_identity(const f_node *in, vector_size_t argidx, f_attribs *attr);

f_node *differentiate_node(const f_node *in, vector_size_t argidx, f_attribs *attr);

int differentiate(f_object *out, const f_object *in, vector_size_t argidx);

void differentiate_command(Object *obj, token_array *args);
