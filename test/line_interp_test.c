#include "../src/env/interpret.h"

int main(void) {
  mString mstr;
  vList vlist = v_from_capacity(10);
  while (0 <= v_get_line(&vlist,&mstr,stdin)) {
    printf("Extracted: ");
    for (size_t i = 0; i < vlist.size; i++) {
      vString word = vlist.data[i];
      printf("\"%s\"[%ld] ", word.ref, word.len);
    }
    printf("\n");
  }
  v_free(&vlist);
  m_deletestr(&mstr);
}


