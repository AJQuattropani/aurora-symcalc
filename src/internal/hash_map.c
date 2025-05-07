#include "hash_map.h"

_value *lookup_value(Map map, _key key) {
  size_t index = hash_key(&key);
  for (_mnode *node = map[index]; node != NULL; node = node->next) {
    if (m_same(&node->key, &key))
      return &node->value;
  }
  return NULL;
}

const _value *lookup_value_const(Map map, const _key key) {
  size_t index = hash_key(&key);
  for (const _mnode *node = map[index]; node != NULL; node = node->next) {
    if (m_same(&node->key, &key)) {
      return &node->value;
    }
  }
  return NULL;
}

_value *acquire_value(Map map, _key key) {
  size_t index = hash_key(&key);
  _mnode **node_addr;
  _mnode *prev_node = NULL;
  _mnode **upstr = &map[index];
  for (node_addr = &map[index]; *node_addr != NULL;
       node_addr = &(*node_addr)->next) {
    prev_node = *node_addr;
    upstr = &(prev_node->next);
    if (m_same(&prev_node->key, &key)) {
      return &prev_node->value;
    }
  }
  // initialize a node in the place
  *node_addr = (_mnode *)malloc(sizeof(_mnode));
  if (NULL == *node_addr) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  **node_addr = (_mnode){.value.ty = NONE,
                         .key = m_from_copy(key),
                         .hash_val = index,
                         .upstr = upstr,
                         .prev = prev_node,
                         .next = NULL};
  return &(**node_addr).value;
}

_mnode *acquire_pair(Map map, _key key) {
  size_t index = hash_key(&key);
  _mnode **node_addr;
  _mnode *prev_node = NULL;
  _mnode **upstr = &map[index];
  for (node_addr = &map[index]; *node_addr != NULL;
       node_addr = &(*node_addr)->next) {
    prev_node = *node_addr;
    upstr = &(prev_node->next);
    if (m_same(&prev_node->key, &key)) {
      return &*prev_node;
    }
  }
  // initialize a node in the place
  *node_addr = (_mnode *)malloc(sizeof(_mnode));
  if (NULL == *node_addr) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  **node_addr = (_mnode){.value.ty = NONE,
                         .key = m_from_copy(key),
                         .hash_val = index,
                         .upstr = upstr,
                         .prev = prev_node,
                         .next = NULL};
  return &(**node_addr);
}

void cinsert(Map map, const char * restrict ckey, _value value) {
  mString key = m_from_cstr(ckey);
  size_t index = hash_key(&key);
  _mnode *prev_node = NULL;
  _mnode **upstr = &map[index];
  _mnode **node_addr;
  for (node_addr = &map[index]; *node_addr != NULL;
       node_addr = &prev_node->next) {
    prev_node = *node_addr;
    upstr = &(prev_node->next);
    if (0 != m_same(&prev_node->key, &key)) {
      fprintf(stderr, "\'%s\' is already defined, cannot be inserted.\n",
              key.cstring);
      exit(-2);
    }
  }
  *node_addr = (_mnode *)malloc(sizeof(_mnode));
  if (NULL == *node_addr) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  (**node_addr) = (_mnode){.key = key,
                           .value = value,
                           .hash_val = index,
                           .upstr = upstr,
                           .prev = prev_node,
                           .next = NULL};
}

void insert(Map map, _key key, _value value) {
  size_t index = hash_key(&key);
  _mnode *prev_node = NULL;
  _mnode **upstr = &map[index];
  _mnode **node_addr;
  for (node_addr = &map[index]; *node_addr != NULL;
       node_addr = &prev_node->next) {
    prev_node = *node_addr;
    upstr = &(prev_node->next);
    if (0 != m_same(&prev_node->key, &key)) {
      fprintf(stderr, "\'%s\' is already defined, cannot be inserted.\n",
              key.cstring);
      exit(-2);
    }
  }
  *node_addr = (_mnode *)malloc(sizeof(_mnode));
  if (NULL == *node_addr) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  (**node_addr) = (_mnode){.key = m_from_copy(key),
                           .value = value,
                           .hash_val = index,
                           .upstr = upstr,
                           .prev = prev_node,
                           .next = NULL};
}

void delete_pair(Map map, _key key) {
  size_t index = hash_key(&key);
  _mnode **node_addr;
  _mnode **next_addr;
  for (node_addr = &map[index]; *node_addr != NULL; node_addr = next_addr) {
    _mnode *node = *node_addr;
    next_addr = &(**node_addr).next;
    if (0 != m_same(&node->key, &key)) {
      *node_addr = *next_addr;
      if (*next_addr != NULL) {
        (*next_addr)->prev = *node_addr;
      }
      mn_destroy(node);
      return;
    }
  }
  printf("skipped deletion of unfound token %s\n", key.cstring);
}

void remove_node(_mnode *restrict node) {
  if (NULL != node->prev) {
    node->prev->next = node->next; // this may be redundant
  }
  if (NULL != node->next) {
    node->next->prev = node->prev;
    node->next->upstr = node->upstr;
  }
  *(node->upstr) = node->next; // change last reference to next always
  mn_destroy(node);
}

void update_map(Map map) {
  for (size_t i = 0; i < STATIC_MAP_SIZE; i++) {
    _mnode **upstr = &map[i]; // bucket
    while (NULL != *upstr) {
      _mnode *rm = *upstr;
      if (TEMP == rm->value.ty || NONE == rm->value.ty) {
        // printf("[Cleanup] Removing node belonging to \"%s\" at %p.\n",
        // (*upstr)->key.cstring, (void*)*upstr);
        remove_node(rm);
        continue;
      }
      upstr = &((*upstr)->next);
    }
  }
}

void empty_map(Map map) {
  for (size_t bkt = 0; bkt < STATIC_MAP_SIZE; bkt++) {
    _mnode *next = NULL;
    for (_mnode *curr = map[bkt]; curr != NULL; curr = next) {
      next = curr->next;
      mn_destroy(curr);
    }
    map[bkt] = NULL;
  }
}

void print_map(Map map) {
  for (size_t bkt = 0; bkt < STATIC_MAP_SIZE; bkt++) {
    if (NULL == map[bkt])
      continue;
    printf("Bucket %ld: ", bkt);
    for (_mnode *curr = map[bkt]; curr != NULL; curr = curr->next) {
      // printf("(%.*s, %d), ", (int)curr->key.size, curr->key.cstring,
      //        curr->value.ty); // may have to change val printing

      printf("(%s, %d), ", curr->key.cstring, curr->value.ty);
    }
    printf("\n");
  }
}

size_t hash_key(const _key * restrict key) {
  size_t val = 0;
  for (size_t i = 0; i < key->size; i++) {
    val += (key->cstring)[i];
  }
  val %= STATIC_MAP_SIZE;
  // printf("hashed [%.*s, %ld] : %ld\n", (int)key->size, key->cstring,
  // key->size, val);
  return val; // float mod
}

void mn_destroy(_mnode * restrict node) {
  m_deletestr(&node->key);
  free_object(&node->value);
  free(node); // free node
}
