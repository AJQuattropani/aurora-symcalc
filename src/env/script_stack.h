#pragma once

#include "../objects/types.h"

#include "../strings/mstring.h"

#define MAX_FILES 16

struct script_stack;
typedef struct script_stack sc_stack;
struct script_stack {
  FILE *open_files[MAX_FILES];
  int count;
};

/*
* Checks if the file is not a std file.
* return: 0 if true, false otherwise
*/
__attribute__((always_inline)) static inline int closeable(FILE *fptr) {
  return (stdin == fptr) || (stdout == fptr) || (stderr == fptr);
}

/*
* Gets the current file. returns NULL if stack is empty.
*/
__attribute__((always_inline)) static inline FILE *get_current_file(sc_stack *stack) {
  if (stack->count <= 0) return NULL;
  return stack->open_files[stack->count - 1];
}

/*
* Pushes an in-use file to the top of the stack.
* THIS IS FOR STD FILESTREAMS ONLY
*/
__attribute__((always_inline)) static inline int push_file(sc_stack *stack, FILE *fptr) {
  if (MAX_FILES <= stack->count) {
    fprintf(stderr, "[Skipped] File open to prevent stack overflow.");
    return 1;
  }
  stack->open_files[stack->count] = fptr; 
  stack->count++;
  return 0;
}

/*
* Opens a file and pushes it to the top of the stack.
* return: 1 on failure, 0 on success
*/
__attribute__((always_inline)) static inline int open_file(sc_stack *stack, const char *file_name) {
  if (MAX_FILES <= stack->count) {
    fprintf(stderr, "[Skipped] File open to prevent stack overflow.");
    return 1;
  }

  FILE *fptr;
  fptr = fopen(file_name, "r"); // read-only
  if (NULL == fptr) {
    fprintf(stderr, "[Skipped] File %s could not be opened.", file_name);
    return 1;
  }
  
  stack->open_files[stack->count] = fptr; 
  stack->count++;
  return 0;
}

/*
* Closes current file and decrements stack pointer.
* return: 1 on empty stack, 0 on success
*/
__attribute__((always_inline)) static inline int pop_file(sc_stack* stack) {
  int curr = stack->count - 1;
  if (curr < 0) {
    return 1;
  }
  FILE **fptr = &stack->open_files[curr];
  if (closeable(*fptr) == 0) {
    fclose(*fptr);
  }
  *fptr = NULL;
  stack->count--;

  return 0;
}

/*
* Closes all files until none are left.
*/
__attribute__((always_inline)) static inline void destroy_stack(sc_stack* stack) {
  while (0 == pop_file(stack)); 
}

