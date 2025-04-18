#include "mstring.h"

mString m_from_cstr(const char *cstr) {
  mString mstr = (mString){.cstring=NULL, .size=0};
  if (NULL == cstr) {
    printf("No reference cstr. Returning an empty string.\n");
    return mstr;
  }
  mstr.size = strlen(cstr);
  mstr.cstring = (char *)malloc(sizeof(char) * (mstr.size + 1));
  if (NULL == mstr.cstring) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  strncpy(mstr.cstring, cstr, mstr.size + 1);
  return mstr;
}

mString m_from_copy(mString mstr) {
  mString copy = (mString)
    {.cstring = (char *)malloc(sizeof(char) * (mstr.size + 1)), 
      .size = mstr.size};
  if (NULL == copy.cstring) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  strncpy(copy.cstring, mstr.cstring, copy.size + 1);
  return copy;
}

mString m_from_size(size_t strlen) {
  mString mstr = (mString){
    .cstring=(char *)calloc(strlen + 1, sizeof(char)),
    .size=strlen};
  if (mstr.cstring == NULL) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  return mstr;
}

void m_deletestr(mString *mstr) {
  free(mstr->cstring);
  mstr->cstring = NULL;
  mstr->size = 0;
}

int m_same(const mString *str1, const mString *str2) {
  if (str1->size != str2->size) {
    return 0;
  }
  int check = strncmp(str1->cstring, str2->cstring, str1->size);
  return !check;
}

