#pragma once

#include "environment.h"
#include "../objects/token_list.h"

// 1. Get a line from the console
// 2. Extract tokens in the form of a list
// 3. Interpret connections between tokens
// 4. if interpret fails, handle 'unknown command'
// 5. if interpret succeeds, carry out instructions

ssize_t m_get_line(mString *ln, FILE *file);

ssize_t v_get_line(vList *vList, mString *buff, FILE *file);

token_array tokenize(Map* map, const vList *vlist);

