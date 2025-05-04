#pragma once
#include <math.h>
#include <stdlib.h>

#include "./allocators/mstring.h"
#include "../objects/object.h"

typedef Object _value;
typedef mString _key;

struct key_value_pair;
typedef struct key_value_pair _kvpair;
struct key_value_pair {
  _key key;
  _value value;
};

struct map_node;
typedef struct map_node _mnode;
struct map_node {
  union {
    _kvpair pair;
    struct {
    _key key;
    _value value;
    };
  };
  size_t hash_val;
  _mnode **upstr;
  _mnode *prev;
  _mnode *next;
};

#define STATIC_MAP_SIZE 4099
// struct map;
// typedef struct map Map;
typedef _mnode *Map[STATIC_MAP_SIZE];

_value *lookup_value(Map map, _key key);

const _value *lookup_value_const(Map map, _key key);

_value *acquire_value(Map map, _key key);

_mnode *acquire_pair(Map map, _key key);

void cinsert(Map map, const char* ckey, _value value);

void insert(Map map, _key key, _value value);

void delete_pair(Map map, _key key);

void remove_node(_mnode *node);

void empty_map(Map map);

void print_map(Map map);

size_t hash_key(const _key *key);

void mn_destroy(_mnode *node);

void update_map(Map map);
