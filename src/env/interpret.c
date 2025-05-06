#include "interpret.h"

ssize_t m_get_line(mString *ln, FILE *file) {
  ssize_t read = getline(&ln->cstring, &ln->size, file);
  return read;
}

ssize_t v_get_line(vList *vlist, mString *buff, FILE *file) {
  v_empty(vlist);
  ssize_t read = m_get_line(buff, file);
  if (0 >= read || NULL == buff->cstring)
    return read;
  char *sp = buff->cstring;
  char *ep = NULL;
  size_t diff;
  while (NULL != (ep = strpbrk(sp, "=^_*/-+() ,%\n\""))) {
    if ('%' == *ep)
      break;
    diff = ep - sp;
    if (0 < diff) {
      v_push_cstr(vlist, sp, diff);
    }
    sp = ep + 1;
    switch (*ep) { // check what the last character is.
    case '\"': {
      if (NULL == (ep = strpbrk(sp, "\""))) {
        fprintf(stderr, "[SKIPPED] Incomplete string.\n");
        v_empty(vlist);
        return read;
      }
      if (0 == (diff = ep - sp)) {
        fprintf(stderr, "[SKIPPED] Empty string.\n");
        v_empty(vlist);
        return read;
      }
      v_push_cstr(vlist, sp, diff);
      sp = ep + 1;
    } break;
    case ',':
      __attribute__((fallthrough));
    case '%':
      __attribute__((fallthrough));
    case ' ':
      break;
    case '-':
      if ('-' == *(ep + 1) || '>' == *(ep + 1)) {
        v_push_cstr(vlist, ep, 2);
        sp++;
        break;
      }
      char num = *(ep + 1) - 0x30;
      if (num >= 0 && num <= 9) {
        break;
      }
      v_push_cstr(vlist, ep, 1);
      break;
    case '\n':
      *ep = '\0';
      break;
    default:
      v_push_cstr(vlist, ep, 1);
      break;
    }
  }

  return read;
}

token_array tokenize(Map map, const vList *vlist) {
  token_array tokens = new_token_array(vlist->size);
  token curr = {NULL, 0};
  for (size_t i = 0; i < vlist->size; i++) {
    if ('(' == vlist->data[i].ref[0]) {
      curr.priority++;
      continue;
    }
    if (')' == vlist->data[i].ref[0]) {
      --curr.priority;
      if (0 > curr.priority)
        goto catch_paren_error;
      continue;
    }
    curr.token =
        acquire_pair(map, (mString){vlist->data[i].ref, vlist->data[i].len});
    if (NONE == curr.token->value.ty) {
      curr.token->value = read_scalar_imp(&curr);
    }
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
