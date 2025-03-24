#include "interpret.h"

struct token_list parse_string_tokens(const char *string, size_t n) {
  struct token_list token_list = {};
  if (string == NULL)
    return token_list;

  char *buff = (char *)malloc((n + 1) * sizeof(char));
  if (buff == NULL) {
    fprintf(stderr, "Buffer allocation failed.");
    exit(-1);
  };
  strncpy(buff, string, n);
  
  struct token tok;

  char *ptr = buff;
  while (ptr < (buff + n)) {
    char c = *ptr;
    switch (c) {
    case '\0':
      goto loopexit;
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      double constant;
      int offset;
      if (sscanf(ptr, "%lf%n", &constant, &offset) == 0) {
        fprintf(stderr, "Unable to scan value at %s", ptr);
        exit(1);
      }
      tok.type = CNS;
      tok.cnst = constant;
      emplace_back(&token_list, &tok);
      ptr += offset;
      break;
    }
    case '+':
    case '-':
    case '*':
    case '/':
    case '^': {
      if (!(tok.type == IDX || tok.type == CNS || tok.type == ')'))
      { // syntax check ensuring operations are balanced by inputs
        fprintf(stderr, "Syntax error: operation missing valid input.");
        exit(3);
      }
      tok.type = BIN;
      tok.id = c;
      emplace_back(&token_list, &tok);
      ptr++;
      break;
    }
    case 'x': {
      tok.type = IDX;
      tok.id = c;
      emplace_back(&token_list, &tok);
      ptr++;
      break;
    }
    case '(':
      if (tok.type == IDX || tok.type == CNS || tok.type == ')')
      { // handles implied multiplication using parens
        tok.type = BIN;
        tok.id = '*';
        emplace_back(&token_list, &tok);
      }
    default:
      tok.priority += -1 * (')' == c) | 1 * ('(' == c);
      tok.id = c;
      if (0 > tok.priority) {
        fprintf(stderr, "Syntax error: Malformed parenthesis.");
        exit(3);
      }
      ptr++;
    }
  }

loopexit:
  free(buff);

  return token_list;
}
