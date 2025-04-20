#pragma once
#include <stdio.h>

#include "types.h"
#include "vector.h"
#include "initializers.h"

f_object build_fobject();

f_node *new_fnode();

Object as_fobject(f_object *fun);

void free_fobject(f_object *fun);

void free_fnode_recurse(f_node *node);

void sprint_function(gString *inp, const f_object *fun);


