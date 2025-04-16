#pragma once

#include "environment.h"

// 1. Get a line from the console
// 2. Extract tokens in the form of a list
// 3. Interpret connections between tokens
// 4. if interpret fails, handle 'unknown command'
// 5. if interpret succeeds, carry out instructions

inline ssize_t m_get_line(mString *ln, FILE* file) {
  ssize_t read = getline(&ln->cstring, &ln->size, file);
  return read;
}

inline ssize_t v_get_line(mString* mstr, vList *list, FILE* file) {

  ssize_t read = m_get_line(mstr, file);
  if ('\n' == mstr->cstring[0] || 0 <= read) return read;
  printf("> %s", mstr->cstring);
  v_empty(list);
  size_t sw = 0, ew = 0;
  
  while (sw < mstr->size) {
    for (ew = ew; ew < mstr->size; ew++) {
      char c = mstr->cstring[ew];
      if (' ' != c && '\n' != c) continue;
      v_push_cstr(list, mstr->cstring + sw, ew); // pushes a new word to string view
      break;
    }
    sw = ew;
  }
  
  return read;
}





