#pragma once

#include <stdlib.h>

#include "tokenlist.h"

/**
 *  Defines a set of various functions for characters to map to, providing instructions
 *  for the tokenizer to map characters it parses to.
 *  
 *  TODO implement ENUM ERROR to explicitly define various types of errors/exits instructed by
 *  the functions.
 *
 *  Additionally, the map of those characters is set in static memory at the bottom. Future implementations
 *  may make this static buffer context-dependent to allow for custom instructions, object declaration, etc.
 *
 *  TIPS: When implementing new instructions, setting context with as much information as possible is non-negotiable.
 *  Doing if-checks also should always involve a check for the TYPE, since id/cnst is a union, and may cause collisions.
 *  For example, if id is read after cnst is set, it will read that character. If cnst is read after id, strange values
 *  may emerge.
 */


// type for function references. returns a positive integer to change offset by, or 0/negative for failures.
typedef int (*reader)(const char* string, struct token_list *list, struct token *context);

/**
 * Instructions for when a parse-exiting character like '\0' is reached.
 */
int p_exit(const char *string, struct token_list *list, struct token *context);

/**
 * Instructions for an ignored character, such as [space].
 */
int p_skip(const char *string, struct token_list *list, struct token *context);

/**
 * NOT IMPLEMENTED YET
 * Instructions for when a open-unary-function character is reached, such as ['\\log'].
 */
int p_function(const char *string, struct token_list *list,
               struct token *context);

/**
 * Instructions for when a binary operator is reached, such as '+', '-', '_' (log),
 * '/', or '^' (pow).
 */
int p_binary(const char *string, struct token_list *list,
             struct token *context);


/**
 * Instructions for handling open parenthesis, handling its role in assigning priority,
 * including implicit multiplcation.
 */
int p_open_paren(const char* string, struct token_list *list, struct token *context);

/**
 * Instructions for handling close parenthesis, returns an error if invalid parenthesis
 * pattern is found during parsing.
 */
int p_close_paren(const char *string, struct token_list *list, struct token *context);

/**
 * Instructions for parsing minus signs as binary operators. First overriden in the case
 * it is actually a negation-operator.
 */
int p_minus(const char *string, struct token_list *list, struct token *context);

/**
 * Instructions for 'e', the natural growth constant.
 */
int p_eulers(const char *string, struct token_list *list, struct token *context);

/**
 * Instructions for generic independent variables, such as 'x'.
 */
int p_variable(const char *string, struct token_list *list, struct token *context);

/**
 * Instructions for numerical constants, such as '-3.515'.
 */
int p_const(const char *string, struct token_list *list, struct token *context);

struct reader_table {
  reader pointers[1<<(8*sizeof(char))];
};

