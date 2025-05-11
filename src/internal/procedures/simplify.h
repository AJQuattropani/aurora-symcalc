#pragma once

#include "../../objects/function.h"
#include "../../readers/functional.h"


void simplify_command(Object *obj, token_array *args);

/*
 * Implementation of simplication procedure.
 */
void simplify_imp(f_object *fun);

/*
 * Swaps consteval term with the first non-consteval term in a commutative
 * chain.
 */
[[nodiscard]] int sort_consteval(const b_opliteral cmp, f_node** cnst_swap, f_node **curr);

[[nodiscard]] int reorder_cleanup_imp(f_node *curr);

[[nodiscard]] int simplify_cleanup_imp(vd_literal *inp_args, f_node *curr, depth_t depth,
                         f_attribs *attr);

[[nodiscard]] int handle_binary_diff_cleanup(vd_literal *inp_args, f_node *curr,
                               depth_t depth, f_attribs *attr);
