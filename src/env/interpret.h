#pragma once

#include "../internal/token_list.h"
#include "environment.h"

ssize_t m_get_line(mString *ln, FILE *file);

ssize_t v_get_line(vList *vList, mString *buff, FILE *file);

token_array tokenize(Map map, const vList *vlist);
