#pragma once
#include <assert.h>
#include "mstring.h"

typedef struct string_qnode {
  mString string;
  struct string_qnode *next;
} string_qnode;

typedef struct string_queue {
  string_qnode *last_ptr;
  string_qnode *curr_ptr;
} string_queue;

string_queue make_queue();

void destroy_queue(string_queue *queue);

const string_qnode *increment_queue(string_queue *queue);

const string_qnode *get_current(string_queue *queue);

void push_back_queue(string_queue *queue, mString *str);

void reset_back_queue(string_queue *queue);

