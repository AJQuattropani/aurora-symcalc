#include "strings.h"

struct string new_string(size_t capacity) {
  struct string new_l = {.size = 0,
                         .capacity = capacity,
                         .data = (char *)calloc(capacity + 1, sizeof(char))};
  if (new_l.data == NULL) {
    fprintf(stderr, "String allocation failed.");
  }
  return new_l;
}

void free_string(struct string *string) {
  string->size = 0;
  string->capacity = 0;
  free(string->data);
  string->data = NULL;
}

void empty_string(struct string *string) {
  memset(string->data, 0, string->size * sizeof(char));
  string->size = 0;
}

void append_back(struct string *string, const char *appendix, size_t len) {
  size_t new_size = string->size + len;
  if (new_size >= string->capacity) { // trigger reallocation
    size_t new_cap =
        2 * (string->capacity +
             len); // make a buffer double the size of the new capacity
    char *temp = (char *)calloc(
        new_cap + 1, sizeof(char)); // calloc handles string termination
    if (temp == NULL) {
      fprintf(stderr, "String reallocation failed.");
      exit(-1);
    }
    memcpy(temp, string->data, string->capacity); // copy in data
    free(string->data);                           // release old buffer
    string->data = temp;                          // set ptr to larger buffer
    string->capacity = new_cap;
  }

  memcpy(string->data + string->size, appendix,
         len); // copy in appendix to end of string
  string->size = new_size;
}

