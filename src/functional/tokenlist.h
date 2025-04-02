#pragma once

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/operation.h"

struct token {
  enum Type type;
  long priority;
  union {
    char id;
    double cnst;
  };
};

struct token_node {
  struct token token;
  struct token_node* next;
};

struct token_list {
  struct token_node* head;
  struct token_node* tail;
  size_t size;
};

/*
 * Frees all elements.
 */
void empty_list(struct token_list* list);

/*
 * Performs an operation accepting each element of the list as an input.
 */
void for_each(const struct token_list *list,
              void (*method)(const struct token_node *));

/*
 * Places an element at the back of the list.
 */
void emplace_back(struct token_list* list, const struct token* token);

/*
* Prints each element of the list in order.
*/
void print(const struct token_list *list);

/*
* Prints an individual token.
*/
void _print(const struct token_node *tok);
