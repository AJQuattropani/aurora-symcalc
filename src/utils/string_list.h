#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * A buffer for viewing strings.
 * Does not create or free strings.
 * References must be updated upon free
 * of buffers pointed to in order to prevent
 * dangling pointers. Assumes all strings are properly
 * null terminated.
 */
struct stringview_list {
  char **data;
  size_t capacity;
  size_t size;
};

/**
 * Creates a new list with the given capacity.
 */
struct stringview_list new_list(size_t n_capacity);

/**
 * Pushes a new element to the list.
 */
void push_view(struct stringview_list *list, char *reference);

/**
 * Erases all data in the list, but does not free the buffer.
 */
void empty_stringlist(struct stringview_list *list);

/**
 * Destroys the buffer, shrinks capacity to 0.
 */
void free_stringlist(struct stringview_list *list);
