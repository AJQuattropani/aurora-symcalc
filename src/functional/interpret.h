#pragma once

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "parse_map.h"
#include "function.h"
#include "tokenlist.h"

#include "../utils/elementary.h"

extern int INITIALIZED_READER_TABLE;

bin_function_t get_binary(char id);

mon_function_t get_unary(char id);

int binary_priority(char t);

const struct token_node *sadmep(const struct token_node *lso,
                                const struct token_node *node);

const struct token_node *min_priority(const struct token_node *lso, const struct token_node *node);

struct token_list parse_string_tokens(const char *string, size_t n);

struct Operation *build_function(const char *string, size_t n);

struct Operation *least_significant_operation(const struct token_node *begin,
                                              const struct token_node *end);




