#include "environment.h"

//////////////////////////////////////
/// INTERNAL FUNCTION DECLARATIONS ///
//////////////////////////////////////

/**
 * Creates a default symbolmap with RESERVED symbols preloaded.
 */
struct symbol_map initialize_symbolmap();

/**
 * Sets flag to exit program and cleans up the environment.
 */
void exit_environment(struct environment *state, char **args,
                      const size_t argn);

/**
 * Empties the symbol map and reinitializes with only RESERVED symbols.
 */
void reset_environment(struct environment *state, char **args,
                       const size_t argn);

/**
 * Changes the file that is being read from.
 */
void run_file(struct environment *state, char **args, const size_t argn);

/**
 * Sets or creates a variable.
 */
void set_var(struct environment *state, char **args, const size_t argn);

/**
 * Sets or creates a function.
 */
void set_func(struct environment *state, char **args, const size_t argn);

/**
 * Print function
 */
void print_generic(struct environment *state, char **args, const size_t argn);

//////////////////////////////////////
//////// IMPLEMENTATION //////////////
//////////////////////////////////////

void execute_from_symbol(struct environment *state, const struct symbol *symbol,
                         char **args, const size_t argn) {
  switch (symbol->type) {
  case RESERVED: {
    reserved state_function = symbol->reserved;
    state_function(state, args, argn);
    break;
  }
  default: {
    break;
  }
  }
}

////////////////// Private

struct symbol_map initialize_symbolmap() {
  struct symbol_map symbols;
  // add RESERVED keytypes
  {
    struct symbol *exit = get_or_add_value(&symbols, "exit");
    exit->type = RESERVED;
    exit->reserved = &exit_environment;
    exit->buff_size = 0;
  }
  {
    struct symbol *reset = get_or_add_value(&symbols, "reset");
    reset->type = RESERVED;
    reset->reserved = &reset_environment;
    reset->buff_size = 0;
  }
  {
    struct symbol *open = get_or_add_value(&symbols, "open");
    open->type = RESERVED;
    open->reserved = &run_file;
    open->buff_size = 0;
  }
  {
    struct symbol *var = get_or_add_value(&symbols, "var");
    var->type = RESERVED;
    var->reserved = &set_var;
    var->buff_size = 0;
  }
  {
    //struct symbol *func = get_or_add_value(&symbols, "func");
    //func->type = RESERVED;
    //func->reserved = &set_func;
  }
  {
    struct symbol *print = get_or_add_value(&symbols, "peak");
    print->type = RESERVED;
    print->reserved = &print_generic;
  }
  return symbols;
}

struct environment initialize_environment() {
  struct environment e = {.status = OK,
                          .symbol_map = initialize_symbolmap(),
                          .current_file = stdin};
  return e;
}

void exit_environment(struct environment *state,
                      __attribute__((unused)) char **args,
                      __attribute__((unused)) const size_t argn) {
  if (stdin != state->current_file) {
    fprintf(stderr, "[skipped] cannot exit program from file.\n");
    return;
  }
  state->status = ESC; // set flag to exit
  if (state->current_file != stdin)
    fclose(state->current_file); // closes any file
  state->current_file = NULL;    // deallocates file pointer
}

void reset_environment(struct environment *state,
                       __attribute__((unused)) char **args,
                       __attribute__((unused)) const size_t argn) {
  if (stdin != state->current_file) {
    fprintf(stderr, "[skipped] cannot exit program from file.\n");
    return;
  }
  empty_symbolmap(&state->symbol_map);
  state->symbol_map = initialize_symbolmap();
  if (state->current_file != stdin)
    fclose(state->current_file);
  state->current_file = stdin;
  printf("[reset] environment variables reset.\n");
}

void run_file(struct environment *state, char **args, const size_t argn) {
  if (state->current_file !=
      stdin) { // open is only a valid command from terminal
    fprintf(stderr, "[open] called from file is invalid.\n");
    return;
  }

  if (2 != argn) {
    fprintf(stderr, "[skipped] accepts only one file.\n");
    return;
  }

  for (size_t i = 1; i < argn; i++) {
    const char *path = args[i];
    if (path == NULL) {
      fprintf(stderr, "Empty string passed to run_file.\n");
      return;
    }
    char *extension;
    if ((extension = strrchr(path, '.')) == NULL) {
      fprintf(stderr, "Invalid path: no file extension supplied.\n");
      return;
    }
    if (strncmp(extension, ".ask", 4) != 0) {
      fprintf(stderr, "Invalid script file: file extension %s not supported.\n",
              extension);
      return;
    }

    {
      FILE *fptr = fopen(path, "r");
      if (fptr == NULL) {
        fprintf(stderr, "File not found: %s\n", path);
        return;
      }
      state->current_file = fptr;
    }
    printf("Beginning to read %s\n", path);

    read_with_state(state);

    fclose(state->current_file);

    state->current_file = stdin;
    printf("Finished reading %s\n", path);
  }
}

void set_var(struct environment *state, char **args, const size_t argn) {
  if (2 >= argn) { // if no value is set
    printf("Please specify a definition to declare a literal.\n");
    return;
  }
  const char *name = args[1];
  struct symbol *var = get_or_add_value(&state->symbol_map, name);

  // add a check for the value being some other type (and to free it? or
  // ignore?)
  if (var->type != LITERAL && var->type != NONE) {
    printf("Attempt to write to non-literal is not supported.\n");
    return;
  }

  // begin reading in values after 2nd arg
  const size_t offs_consumed_args = 2;
  const size_t vec_size = argn - offs_consumed_args;

  double *buff = (double *)realloc(var->data, vec_size * (sizeof(double)));
  if (NULL == buff) {
    fprintf(stderr, "Buffer reallocation failed for variable.\n");
    return;
  }

  char **def = &args[offs_consumed_args]; 
  for (size_t i = 0; i < vec_size; i++) {
    if (0 >= sscanf(def[i], "%lf", &buff[i])) { // reads in a double from each
      printf("Improper definition provided for input %ld.\n", i);
      buff[i] = 0;
      continue;
    }
  }
  
  var->data = buff;
  var->type = LITERAL;
  var->buff_size = vec_size;

}

void set_func(struct environment *state, char **args, const size_t argn) {
  



}

void print_generic(struct environment *state, char **args, const size_t argn) {
  const size_t offs_consumed_args = 1;
  if (offs_consumed_args >= argn) {
    printf("No arguments provided.\n");
  }
  
  char **names = &args[offs_consumed_args];
  
  struct string output = new_string(argn*15);
  for (size_t i = 0; i < argn - offs_consumed_args; i++) {
    const struct symbol* key = get_value(&state->symbol_map, names[i]);
    if (key == NULL) {
      const char* app = "NONE ";
      append_back(&output, app, strlen(app));
      continue;
    }
    switch (key->type) {
      case NONE: {
        const char* app = "UNDEFINED ";
        append_back(&output, app, strlen(app));
        break;
      }
      case RESERVED: {
        const char* app = "RESERVED ";
        append_back(&output, app, strlen(app));
        break;
      }
      case LITERAL: {
        append_back(&output, "[", 1);
        for (size_t j = 0; j < key->buff_size; j++) {
          char dbl_buff[26];
          snprintf(dbl_buff, 26, "%lf,", ((double*)key->data)[j]);
          append_back(&output, dbl_buff, strlen(dbl_buff));
        }
        append_back(&output, "] ", 2);
        break;
      }
      case FUNCTION: { // todo implement PRINT function.
        const char* app = "FUNCTION ";
        append_back(&output, app, strlen(app));
        break;
      }
    }
  }
  printf("Peak > %s \n", output.data);
  
  free_string(&output);

}


