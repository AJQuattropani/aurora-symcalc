#pragma once

#include <stdio.h>

#include "types.h"

Object as_vdliteral(vd_literal *value);

void free_vdliteral(vd_literal *lit);

vd_literal alloc_vdliteral(size_t n);
