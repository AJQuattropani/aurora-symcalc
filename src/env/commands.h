#pragma once

#include "environment.h"
#include "../objects/hash_map.h"

/*
 * Applicable functions for CONTEXT type
 */

void exit_env(env *context, vList *args);

void reset_env(env *context, vList *args);

//void open_file(env *context, vList *args);


