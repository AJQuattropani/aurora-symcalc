#pragma once

#include "environment.h"
#include "../objects/hash_map.h"
#include "../objects/token_list.h"

/*
 * Applicable functions for CONTEXT type
 */

void exit_env(env *context, const token_array *args);

void reset_env(env *context, const token_array *args);

//void open_file(env *context, vList *args);


