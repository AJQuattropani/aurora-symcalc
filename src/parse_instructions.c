#include "parse_instructions.h"

/**
  * INTERNAL IMPLEMENTATION METHODS
  */
// checks if last character was an unchecked minus sign
int is_negative(struct token *context) {
  return (context->id == '-') && (context->type == UNR);
}

// checks if the last character indicates a valid input for a binary operator
int valid_lvalue(struct token *context) {
  return context->type == CNS || context->type == IDX || (context->id == ')' && context->type == UNR);
}

/**
 * Instructions that check and execute if there is an implicit-multiplication,
 * essentially for cases like 5x, x(x+1).
 */
int p_implicit_mult([[maybe_unused]] const char *string, struct token_list *list,
                    struct token *context) {
  if (valid_lvalue(context)) {
    context->type = BIN;
    context->id = '*';
    emplace_back(list, context);
  }
  return 1;
}

/**
  * FUNCTION DEFINITIONS
  */

int p_exit([[maybe_unused]] const char *string, [[maybe_unused]] struct token_list *list, [[maybe_unused]] struct token *context) {
  return 0; // 0 is the canonical value of "exit loop:"
}

int p_skip([[maybe_unused]] const char *string, [[maybe_unused]] struct token_list *list, [[maybe_unused]] struct token *context) {
  return 1; // the default ignore instruction is to "move ptr right by 1"
}

int p_variable(const char *string, struct token_list *list,
               struct token *context) {
  if (is_negative(context)) { // pushes back unary minus if unhandled negative preceeds it
    emplace_back(list, context);
  }
  context->type = IDX;
  context->id = *string;
  emplace_back(list, context);
  return 1;
}

int p_binary(const char *string, struct token_list *list,
             struct token *context) {
  if (!valid_lvalue(context)) return -1; // throws an error
  context->type = BIN;
  context->id = *string;
  emplace_back(list, context);
  return 1;
}

// TO DO IMPLEMENT. EXITS.
int p_function([[maybe_unused]]const char *string, [[maybe_unused]]struct token_list *list,
               [[maybe_unused]]struct token *context) {
  int skip = 0;
  return skip;
}

int p_open_paren(const char* string, struct token_list *list, struct token *context) {
  int result = p_implicit_mult(string, list, context); 
  if (result) result = 0; 
  // REEXAMINE THIS LINE.
  // if (0 >= result) return result; // if implicit mult fails return its fail value
  // check for parenthetical multiplication and 
  context->priority++;
  context->type = UNR; // unary used as type flag for parens
  context->id = '(';
  return 1; // shift ptr right 1
}

int p_close_paren([[maybe_unused]] const char *string, [[maybe_unused]]struct token_list *list, struct token *context) {
  if (0 > context->priority) return -1; // if priority goes negative, invalid expression
  context->priority--;
  context->type = UNR; // unary used as type flag for parens
  context->id = ')';
  return 1; // shift ptr right 1
}

int p_minus(const char *string, struct token_list *list,
            struct token *context) {
  int x = p_binary(string, list, context);
  if(x > 0) return x; // attempt to treat as binary function
  // proceeds if this fails (error)
  context->type = UNR;
  context->id = '-';
  return 1; // if not binary function, flag for context as an implicit negative op
}

int p_eulers([[maybe_unused]]const char *string, struct token_list *list, struct token *context) {
  context->type = CNS;
  context->cnst = 2.718281828459045;
  emplace_back(list, context);
  return 1;
}

int p_const(const char *string, struct token_list *list,
            struct token *context) {
  double constant;
  int offset;
  if(sscanf(string, "%lf%n", &constant, &offset) == 0) {
    return -6;
  }
  if (is_negative(context)) constant *= -1.0;
  context->type = CNS;
  context->cnst = constant;
  emplace_back(list, context);
  return offset;
}




