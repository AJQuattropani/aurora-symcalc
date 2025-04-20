#include "gstring.h"

gString g_from_cstr(const char *cstr) {
  gString gstr = (gString){.cstring=NULL, .size=0, .capacity=0};
  if (NULL == cstr) {
    printf("No reference cstr. Returning an empty string.\n");
    return gstr;
  }
  gstr.size = strlen(cstr);
  gstr.capacity = 2 * gstr.size;
  gstr.cstring = (char *)malloc(sizeof(char) * (gstr.capacity+1));
  if (NULL == gstr.cstring) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  strncpy(gstr.cstring, cstr, gstr.size + 1);
  return gstr;
}

gString g_from_copy(const gString *gstr) {
  gString copy = (gString){
    .cstring = (char *)malloc(sizeof(char) * (gstr->capacity + 1)),
    .size = gstr->size,
    .capacity = gstr->capacity};
  if (NULL == copy.cstring) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  strncpy(copy.cstring, gstr->cstring, copy.size + 1);
  return copy;
}

gString g_from_capacity(size_t capacity) {
  gString gstr = (gString){
    .cstring = (char *)calloc(capacity + 1, sizeof(char)), 
    .size=0, 
    .capacity=capacity}; 
  if (gstr.cstring == NULL) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return gstr;
}

__attribute__((always_inline)) inline void g_deletestr(gString *gstr) {
  gstr->size = 0;
  gstr->capacity = 0;
  free(gstr->cstring);
  gstr->cstring = NULL;
}

__attribute__((always_inline)) inline void g_empty(gString *gstr) {
  memset(gstr->cstring, 0, gstr->size * sizeof(char));
  gstr->size = 0;
}

__attribute__((always_inline)) inline void g_append_back(gString *gstr, const char *appendix, size_t len) {
  size_t new_size = gstr->size + len;
  if (new_size >= gstr->capacity) { // trigger reallocation
    size_t new_cap =
      2 * (gstr->capacity + len);
    char *temp = (char *)realloc(gstr->cstring, (new_cap+1) * sizeof(char));
    // make a buffer double the size of the new capacity
    if (NULL == temp) {
      fprintf(stderr, "realloc failed in %s\n", __func__);
      exit(1);
    }
    gstr->cstring = temp;
    gstr->capacity = new_cap;
  }

  strncpy(gstr->cstring + gstr->size, appendix, len + 1); // copy in appdx to end of string
  gstr->size = new_size;
}

__attribute__((always_inline)) inline void g_append_back_c(gString *gstr, const char *appendix) {
  size_t len = strlen(appendix);
  size_t new_size = gstr->size + len;
  if (new_size >= gstr->capacity) { // trigger reallocation
    size_t new_cap =
      2 * (gstr->capacity + len);
    char *temp = (char *)realloc(gstr->cstring, (new_cap+1) * sizeof(char));
    // make a buffer double the size of the new capacity
    if (NULL == temp) {
      fprintf(stderr, "realloc failed in %s\n", __func__);
      exit(1);
    }
    gstr->cstring = temp;
    gstr->capacity = new_cap;
  }

  strncpy(gstr->cstring + gstr->size, appendix, len + 1); // copy in appdx to end of string
  gstr->size = new_size;
}

