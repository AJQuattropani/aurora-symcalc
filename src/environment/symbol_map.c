#include "symbol_map.h"

unsigned long int hash(const char *key) {
  unsigned long int sum = 0;
  for (size_t i = 0; i < strlen(key); i++) {
    sum += key[i];
  }
  return sum % BUCKETS;
}

const struct symbol *get_value(const struct symbol_map *map, const char *key) {
  for (struct symbol_node *bucket = map->buckets[hash(key)]; bucket != NULL; bucket = bucket->next) {
    if (strcmp(key, bucket->key) == 0)
      return &bucket->value;
  }
  return NULL;
}

struct symbol *get_or_add_value(struct symbol_map *map, const char *key) {
  struct symbol_node **ptr_addr = &map->buckets[hash(key)];
  for (;
       *ptr_addr != NULL; ptr_addr = &((**ptr_addr).next)) {
    if (strcmp(key, (**ptr_addr).key) == 0)
      return &((**ptr_addr).value);
  }
  // if nothing is returned, initialize new value replacing reference to NULL at
  // the location
  *ptr_addr = (struct symbol_node *)calloc(1, sizeof(struct symbol_node));
  if (NULL == *ptr_addr)
    exit(-1); // if creation of key-value pair fails
  (**ptr_addr).key = (char *)malloc((strlen(key) + 1) * sizeof(char));
  if (NULL == (**ptr_addr).key)
    exit(-1); // if creation of buffer for storing key is lost
  strcpy((**ptr_addr).key, key);
  return &((**ptr_addr).value);
}

void empty_symbolmap(struct symbol_map *map) {
  for (size_t i = 0; i < BUCKETS; i++) {
    struct symbol_node *next = NULL;
    for (struct symbol_node *head = map->buckets[i]; head != NULL;
         head = next) {
      next = head->next;
      free(head->key); // no check: if there is a node without a key, the
                       // program should crash
      struct symbol* val = &(head->value);
      if (NULL != val->data) {
        switch (val->type) {
          case LITERAL:
            free(val->data);
          break;
          case FUNCTION:
            a_free_recursive((struct Operation*)val->data);
          break;
          case RESERVED: break;
          default: break;
        }
      }
      free(head); // this needs to be replaced in the future with a general free function
    }
    map->buckets[i] = NULL;
  }
}
