#include "interpret.h"

struct token_list parse_string_tokens(const char *string, size_t n) {
  static int INITIALIZED_READER_TABLE = 0;
  static struct reader_table READER_TABLE;
  if (!INITIALIZED_READER_TABLE) {
    init_parse_map(&READER_TABLE);
    INITIALIZED_READER_TABLE = 1;
  }
  char *buff = (char *)malloc((n + 1) * sizeof(char));
  
  //context 
  char *ptr = buff; // current location on the buffer over which
  struct token_list token_list = {};
  struct token tok = {.type=BEG};
  
  if (string == NULL)
    return token_list;
  // copy string input into buffer
  if (buff == NULL) {
    fprintf(stderr, "Buffer allocation failed.\n");
    exit(-1);
  };
  strncpy(buff, string, n);
  
  while (ptr < (buff + n)) {
    char c = *ptr;
    reader perform = READER_TABLE.pointers[(size_t)c];
    if (perform == NULL) {
      printf("Unrecognized value at %s\n", ptr);
      empty_list(&token_list);
      break;
    }
    int status = perform(ptr, &token_list, &tok);
    if (0 == status) break;
    if (0 > status) {
      printf("\"%s\" Returned an error: %d at \'%c\'\n", __func__, status, c);
      print(&token_list);
      printf("Context:\n");
      printf("{%ld,", tok.priority);
      if(tok.type != CNS) printf("%c}, ", tok.id);
      else
        printf("%lf}, ", tok.cnst);
      printf("%d \n", tok.type);
      empty_list(&token_list);
      break;
    }
    ptr += status;
  }
  
  if(tok.priority != 0) {
    printf("Incomplete or invalid parenthesis in expression!\n");
    empty_list(&token_list);
  }

  free(buff);

  return token_list;
}
