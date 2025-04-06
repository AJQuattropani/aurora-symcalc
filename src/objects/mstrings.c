#include "mstrings.h"

mString from_cstr(const char *cstr) {
  mString mstr = {NULL, 0};
  if (NULL == cstr) {
    printf("No reference cstr. Returning an empty string.\n");
    return mstr;
  }
  mstr.size = strlen(cstr);
  mstr.string = (char *)malloc(sizeof(char) * (mstr.size + 1));
  if (NULL == mstr.string) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  strcpy(mstr.string, cstr);
  return mstr;
}

mString from_copy(mString mstr) {
  mString copy = {(char *)malloc(sizeof(char) * (mstr.size + 1)), mstr.size};
  if (NULL == copy.string) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  strcpy(copy.string, mstr.string);
  return copy;
}

mString from_size(size_t strlen) {
  mString mstr = {NULL, strlen};
  mstr.string = (char *)calloc(strlen + 1, sizeof(char));
  if (mstr.string == NULL) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return mstr;
}

void delete_mstr(mString *mstr) {
  free(mstr->string);
  mstr->string = NULL;
  mstr->size = 0;
}

int msame(const mString *str1, const mString *str2) {
  if (str1->size != str2->size) {
    return 0;
  }
  int check = strcmp(str1->string, str2->string);
  return !check;
}

