#include "stringqueue.h"

string_qnode *new_qnode(const mString *str, string_qnode *next);

string_queue make_queue() {
  return (string_queue){.last_ptr = NULL, .curr_ptr = NULL};
}

void destroy_queue(string_queue *queue) {
  string_qnode *next = NULL;
  for (string_qnode *top = queue->last_ptr; top != NULL; top = next) {
    next = top->next;
    free(top);
  }
  *queue = (string_queue){0};
}

const string_qnode *increment_queue(string_queue *queue) {
  assert(NULL != queue->last_ptr);

  if (NULL == queue->curr_ptr) {
    reset_back_queue(queue);
    return queue->curr_ptr;
  }
  queue->curr_ptr = queue->curr_ptr->next;
  return queue->curr_ptr;
}

const string_qnode *get_current(string_queue *queue) { return queue->curr_ptr; }

string_qnode *new_qnode(const mString *str, string_qnode *next) {
  string_qnode *qnode = (string_qnode *)malloc(sizeof(string_qnode));
  if (NULL == qnode) {
    fprintf(stderr, "malloc failed in %s\n", __func__);
    exit(1);
  }
  *qnode = (string_qnode){.string = *str, .next = next};
  return qnode;
}

void push_back_queue(string_queue *queue, mString *str) {
  string_qnode *new_top = new_qnode(str, queue->last_ptr);
  queue->last_ptr = new_top;

  *str = (mString){0};
}

void reset_back_queue(string_queue *queue) {
  queue->curr_ptr = queue->last_ptr;
}
