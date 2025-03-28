#pragma once

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "parse_map.h"
#include "elementary.h"
#include "function.h"
#include "tokenlist.h"

extern int INITIALIZED_READER_TABLE;

struct token_list parse_string_tokens(const char *string, size_t n);

struct Operation *build_function(const struct token_list *tokens);
