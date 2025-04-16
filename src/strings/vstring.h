#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct view_string;
typedef struct view_string vString;
struct view_string {
  char* ref;
  size_t len;
};

vString v_view(char *ref);

struct view_list;
typedef struct view_list vList;
struct view_list {
  vString* data;
  size_t capacity;
  size_t size;
};

vList v_from_capacity(size_t n_capacity);

void v_push_cstr(vList *list, char *ref, size_t len);

void v_empty(vList *list);

void v_free(vList *list);
