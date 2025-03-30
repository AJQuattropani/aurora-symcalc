#pragma once

#include <stdlib.h>
#include <string.h>

// BUILD A HASHMAP THAT CONTAINS ALL THE 

enum symbol_type {
  RESERVED,
  LITERAL,
  FUNCTION
};

struct symbol{
  void* data;
  enum symbol_type type;
};

struct symbol_node{
  char* key;
  struct symbol value;
  struct symbol_node* next;
};

#define BUCKETS 52

struct symbol_map {
  struct symbol_node *buckets[BUCKETS];
};

/**
* Simple integer mod hash.
*/
unsigned long int hash(const char *key);

/**
* Searches the map for the key. If none is found, returns NULL.
*/
const struct symbol *get_value(const struct symbol_map *map, const char *key);

/**
* Searches the map for the key. If none is found, a new key-value pair is created.
*/
struct symbol *get_or_add_value(struct symbol_map *map, const char *key);

/**
* Empties the map. This is necessary before deallocating symbol map.
*/
void empty_symbolmap(struct symbol_map *map);
