#pragma once

#include <stdio.h>

#include "types.h"
#include "../strings/gstring.h"

Object as_vdliteral(vd_literal *value);

void free_vdliteral(vd_literal *lit);

vd_literal alloc_vdliteral(size_t n);

void sprint_vector(gString *inp, const vd_literal *value);
