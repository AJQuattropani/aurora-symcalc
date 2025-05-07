#include "gstring.h"

gString g_from_capacity(size_t capacity) {
  // gString g = {.cstring=NULL,.size=capacity};
  gString g = {.cstring = NULL, .size = 0, .capacity = capacity};
  char *temp = (char *)calloc(capacity, sizeof(char));
  g.cstring = temp;
  return g;
}

void g_deletestr(gString *restrict gstr) {
  free(gstr->cstring);
  gstr->size = 0;
}

void g_empty(gString *gstr) {
  memset(gstr->cstring, '\0', gstr->size);
  gstr->size = 0;
}

/*__attribute__((always_inline)) inline*/ void
g_append_back(gString *restrict gstr, const char *restrict appendix, size_t len) {
  if (gstr->size + len + 1 > gstr->capacity) {
    char *temp = (char *)realloc(gstr->cstring, (gstr->size + len) * 2 + 1);
    if (NULL == temp) {
      fprintf(stderr, "realloc failed in %s", __func__);
    }
    gstr->cstring = temp;
    gstr->capacity = (gstr->size + len) * 2 + 1;
    gstr->cstring[gstr->capacity - 1] = '\0';
  }
  memcpy(gstr->cstring + gstr->size, appendix, len);
  gstr->size = gstr->size + len;
}

/*__attribute__((always_inline)) inline*/ void
g_append_back_c(gString *restrict gstr, const char *restrict appendix) {
  size_t len = strlen(appendix);
  g_append_back(gstr, appendix, len);
}
