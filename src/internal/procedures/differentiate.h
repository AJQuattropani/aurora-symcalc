#pragma once

#include <assert.h>

#include "../../objects/function.h"
#include "../../readers/functional.h"
#include "../token_list.h"
#include "operators.h"
#include "simplify.h"

[[nodiscard]] f_node *differentiate_node(const f_node *in, vector_size_t argidx,
                           f_attribs *attr);

[[nodiscard]] int differentiate(f_object *out, const f_object *in, vector_size_t argidx);

void differentiate_command(Object *obj, token_array *args);
