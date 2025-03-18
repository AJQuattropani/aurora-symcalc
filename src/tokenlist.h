#pragma once

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "operation.h"

struct token {
  enum Type type;
  long priority;
  union {
    char id;
    double cnst;
  };
};

struct token_list {
  struct token token;
  struct token_list* next;
};

struct token_list *new_list();

void free_list(struct token_list *list);

void for_each(struct token_list *list, void (*method)(const struct token_list *));

void emplace_front(struct token_list **list, struct token);

void combine_front(struct token_list **list, struct token_list *appendix);

void print(const struct token_list *tok);
