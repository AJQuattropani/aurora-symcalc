#pragma once
#include <math.h>
#include <stdlib.h>

#include "mstrings.h"

typedef int _value;
typedef mString _key;

struct map_node;
typedef struct map_node _mnode;
struct map_node {
  _key key;
  _value value;
  _mnode *prev;
  _mnode *next;
};

//#define STATIC_MAP_SIZE 4099
#define STATIC_MAP_SIZE 2
// struct map;
// typedef struct map Map;
typedef _mnode *Map[STATIC_MAP_SIZE];

_value *lookup_value(const Map *map, _key key);

const _value *lookup_value_const(const Map *map, _key key);

_value *acquire_value(Map *map, _key key);

void insert(Map *map, _key key, _value value);

void delete_pair(Map *map, _key key);

void empty_map(Map *map);

void print_map(Map *map);

size_t hash_key(const _key *key);
