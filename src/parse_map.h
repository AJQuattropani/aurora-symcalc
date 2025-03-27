#pragma once

#include "parse_instructions.h"

/**
  * Creates a reader table in static memory, this translates characters into actions
  * that affect the context and output list of the string tokenizer.
  */
static int INITIALIZED_READER_TABLE = 0;
void init_parse_map(struct reader_table *table);
