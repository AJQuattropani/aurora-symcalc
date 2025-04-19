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
  size_t diff; 
  while (NULL != (ep = strpbrk(sp, " \n()"))) {
    diff = ep - sp;
    if (0 < diff) {
      v_push_cstr(vlist, sp, diff);
    }
    sp = ep + 1;
    if (')' == *ep || '(' == *ep) {
      v_push_cstr(vlist, ep, 1);
    }
  }
  sp[read-1] = '\0';

  return read;
}

token_array tokenize(Map* map, const vList *vlist) {
  token_array tokens = new_token_array(vlist->size);
  token curr = {NULL, 0};
  for (size_t i = 0; i < vlist->size; i++) {
    if ('(' == vlist->data[i].ref[0]) {
      curr.priority++;
      continue;
    }
    if (')' == vlist->data[i].ref[0]) {
      --curr.priority;
      if (0 > curr.priority) goto catch_paren_error;
      continue;
    }
    curr.token = acquire_pair(map, (mString){vlist->data[i].ref,vlist->data[i].len});
    push_token_back(&tokens, &curr);
  }
  if (0 != curr.priority) {
    goto catch_paren_error;
  }
  return tokens;
catch_paren_error:
    fprintf(stderr, "Invalid or malformed parenthesis.\n");
    destroy_token_array(&tokens);
    return tokens;
}

