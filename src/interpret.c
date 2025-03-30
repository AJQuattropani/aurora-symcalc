#include "interpret.h"

struct token_list parse_string_tokens(const char *string, size_t n) {
  static int INITIALIZED_READER_TABLE = 0;
  static struct reader_table READER_TABLE;
  if (!INITIALIZED_READER_TABLE) {
    init_parse_map(&READER_TABLE);
    INITIALIZED_READER_TABLE = 1;
  }
  char *buff = (char *)malloc((n + 1) * sizeof(char));

  // context
  char *ptr = buff; // current location on the buffer over which
  struct token_list token_list = {};
  struct token tok = {.type = BEG};

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
    if (0 == status)
      break;
    if (0 > status) {
      printf("\"%s\" Returned an error: %d at \'%c\'\n", __func__, status, c);
      print(&token_list);
      printf("Context:\n");
      printf("{%ld,", tok.priority);
      if (tok.type != CNS)
        printf("%c}, ", tok.id);
      else
        printf("%lf}, ", tok.cnst);
      printf("%d \n", tok.type);
      empty_list(&token_list);
      break;
    }
    ptr += status;
  }

  if (tok.priority != 0) {
    printf("Incomplete or invalid parenthesis in expression!\n");
    empty_list(&token_list);
  }

  free(buff);

  return token_list;
}

struct Operation *build_function(const char *string, size_t n) {
  struct token_list token_list = parse_string_tokens(string, n);
  string = NULL;

  if (NULL == token_list.head || NULL == token_list.tail ||
      0 == token_list.size) {
    fprintf(stderr, "Invalid token_list.");
    return NULL;
  }

  struct Operation *result =
      least_significant_operation(token_list.head, NULL);

  empty_list(&token_list);

  return result;
}



struct Operation *least_significant_operation(const struct token_node *begin,
                                              const struct token_node *end) {
  if (begin->next == end) {
    if (IDX == begin->token.type)
      return NULL; // base case 1: identity operation
    if (CNS != begin->token.type) {
      fprintf(stderr, "Warning: an invalid base token was encountered. Data "
                      "will be treated as a constant.\n");
      _print(begin);
    }
    struct Operation *base_case = a_operation(1);
    base_case->_default = begin->token.cnst;
    return base_case; // base case 2: constant function
  }

  const struct token_node *lso = begin;
  // FIND THE LEAST SIGNIFICANT OPERATION
  for (const struct token_node *node = begin; node != end; node = node->next) {
    if (node->token.priority != lso->token.priority) {
      lso = min_priority(lso, node);
      continue;
    }
    if (node->token.type < lso->token.type)
      continue;
    if (BIN == lso->token.type) {
      lso = sadmep(lso, node);
      continue;
    }
    lso = node;
  }
  
  printf("Found lso token: ");
  _print(lso);
  printf("\n");

  if (lso->token.type == UNR) {
    struct Operation *oper = a_operation(1);
    oper->post_function = get_unary(lso->token.id);
    if (NULL == oper->post_function) {
      fprintf(stderr, "Error: no instruction belongs to unary token.\n");
      exit(-1);
    }
    oper->binary_function = &b_add;
    oper->_left = a_operation(1); // initializes a constant 0, can be optimized away later
    oper->_right = least_significant_operation(begin->next, end); // check rightward
    return oper;
  }

  if (lso == begin) {
    fprintf(stderr, "Error: an invalid operation token was encountered.");
    exit(-1);
  }
  
  struct Operation *oper = a_operation(1);
  oper->_left = least_significant_operation(begin, lso); // split list to left
  oper->_right = least_significant_operation(lso->next, end); // split list to right
  oper->binary_function = get_binary(lso->token.id);
  if (NULL == oper->binary_function) {
    fprintf(stderr, "Error: no instruction belongs to binary token.");
    exit(-1);
  }
  return oper;
}
const struct token_node *sadmep(const struct token_node *lso,
                                const struct token_node *node) {
  // compiler optimizes away jump
  if (binary_priority(lso->token.id) > binary_priority(node->token.id))
    return node;
  return lso;
}

const struct token_node *min_priority(const struct token_node *lso, const struct token_node *node) {
  if (lso->token.priority > node->token.priority) return node;
  return lso;
}

int binary_priority(char t) {
  // switch statement compiles with jumps in godbolt for -O3, so this was
  // favored.
  return -1 + (t == '+' || t == '-') * 1 + (t == '*' || t == '/') * 2 +
         (t == '^' || t == '_') * 3;
}

bin_function_t get_binary(char id) {
  switch (id) {
  case '+':
    return b_add;
  case '-':
    return b_sub;
  case '*':
    return b_mult;
  case '/':
    return b_div;
  case '^':
    return b_pow;
  case '_':
    return b_log;
  default:
    return NULL;
  }
}

mon_function_t get_unary(char id) {
  switch (id) {
  case 's':
    return u_sin;
  case 'c':
    return u_cos;
  case 't':
    return u_tan;
  case 'l':
    return u_nlog;
  case '/':
    return u_inv;
  case 'T':
    return u_cot;
  case 'C':
    return u_sec;
  case 'S':
    return u_csc;
  case -'c':
    return u_arccos;
  case -'s':
    return u_arcsin;
  case -'t':
    return u_arctan;
  case -'T':
    return u_arccot;
  case -'C':
    return u_arcsec;
  case -'S':
    return u_arccsc;
  default:
    return NULL;
  }
}

