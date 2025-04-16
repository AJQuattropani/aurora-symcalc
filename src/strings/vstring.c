#include "vstring.h"

vList v_from_capacity(size_t capacity) {
  vList list = (vList){
    .data = (vString*)malloc(capacity * sizeof(vString)),
    .capacity = capacity,
    .size = 0
  };
  if (NULL == list.data) {
    fprintf(stderr, "malloc failed in %s", __func__);
    exit(1);
  }
  return list;
}

void v_push_cstr(vList *list, char *ref, size_t len) {
  if (list->size >= list->capacity) {
    size_t new_capacity = list->size * 2;
    vString* temp = (vString*)realloc(list->data, new_capacity * sizeof(vList));
    if (NULL == temp) {
      fprintf(stderr, "realloc failed in %s", __func__);
      exit(1);
    }
    list->data = temp;
    list->capacity = new_capacity;
  }
  list->data[list->size] = (vString){.ref=ref,.len=len};
  list->size += 1;
}

void v_empty(vList *list) {
  if (NULL == list->data) {
    fprintf(stderr, "skipping attempt to empty freed list");
    return;
  }
  memset(list->data, 0, list->capacity * sizeof(vString));
  list->size = 0;
}

void v_free(vList *list) {
  list->capacity = 0;
  list->size = 0;
  if (NULL == list->data) {
    fprintf(stderr, "skipping attempt to re-free list");
    return;
  }
  free(list->data);
  list->data = NULL;
}

vString v_view(char *ref) {
  return (vString){.ref = ref, .len = strlen(ref)};
}

