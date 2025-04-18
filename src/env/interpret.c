#include "interpret.h"

ssize_t m_get_line(mString *ln, FILE *file) {
ssize_t read = getline(&ln->cstring, &ln->size, file);
  return read;
}

ssize_t v_get_line(vList *vlist, mString *buff, FILE *file) {
  v_empty(vlist);
  ssize_t read = m_get_line(buff, file);
  if (0 >= read || NULL == buff->cstring) return read;
  char *sp = buff->cstring;
  char *ep = NULL;
  while (NULL != (ep = strpbrk(sp, " \n"))) {
    *ep = '\0';
    size_t diff = ep - sp;
    if (diff > 0) {
      v_push_cstr(vlist, sp, diff);
    }
    sp = ep + 1;
    
  }
  return read;
}

