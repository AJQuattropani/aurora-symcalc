#pragma once

#include "parse_instructions.h"

/**
  * Creates a reader table in static memory, this translates characters into actions
  * that affect the context and output list of the string tokenizer.
  */
void init_parse_map(struct reader_table *table);
