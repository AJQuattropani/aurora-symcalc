#include "../src/env/interpret.h"

int main(void) {
  Map map;
  default_map(&map);
  mString mstr;
  vList vlist = v_from_capacity(10);
  while (0 <= v_get_line(&vlist,&mstr,stdin)) {
    printf("Extracted: ");
    for (size_t i = 0; i < vlist.size; i++) {
      vString word = vlist.data[i];
      printf("\"%.*s\"[%ld] ", (int)word.len, word.ref, word.len);
    }
    printf("\n");
    token_array arr = tokenize(&map, &vlist);
    destroy_token_array(&arr);
  }
  v_free(&vlist);
  m_deletestr(&mstr);
  empty_map(&map);
  return 0;
}


