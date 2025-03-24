#pragma once

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "elementary.h"
#include "function.h"
#include "tokenlist.h"

struct token_list parse_string_tokens(const char *string, size_t n);

struct Operation *build_function(const struct token_list* tokens);




