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
void fetch_file(struct environment *state, char **args, const size_t argn);

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
  }
  {
    struct symbol *reset = get_or_add_value(&symbols, "reset");
    reset->type = RESERVED;
    reset->reserved = &reset_environment;
  }
  {
    struct symbol *open = get_or_add_value(&symbols, "open");
    open->type = RESERVED;
    open->reserved = &fetch_file;
  }
  return symbols;
}

struct environment initialize_environment(){
  struct environment e = {.status = OK,
                          .symbol_map = initialize_symbolmap(),
                          .current_file = stdin};
  return e;
}

void exit_environment(struct environment *state,__attribute__((unused)) char **args,
                     __attribute__((unused)) const size_t argn) {
  state->status = ESC;                 // set flag to exit
  if (state->current_file != stdin)
    fclose(state->current_file); // closes any file
  state->current_file = NULL;    // deallocates file pointer
}

void reset_environment(struct environment *state,__attribute__((unused)) char **args,
                      __attribute__((unused)) const size_t argn) {
  empty_symbolmap(&state->symbol_map);
  state->symbol_map = initialize_symbolmap();
  if (state->current_file != stdin)
    fclose(state->current_file);
  state->current_file = stdin;
  printf("[reset] environment variables reset.\n");
}

void fetch_file(struct environment *state, char **args, const size_t argn) {
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
    
    FILE* fptr = fopen(path, "r");
    if (fptr == NULL) {
      fprintf(stderr, "File not found: %s\n", path);
      return; 
    }
    printf("Beginning to read %s\n", path);
    
    state->current_file = fptr;
    fptr = NULL;

    char *line = NULL;
    size_t size = 0;
    for (ssize_t read = getline(&line, &size, state->current_file); read >= 0;
         read = getline(&line, &size, state->current_file)) {
      printf("> %s", line);
    }

    free(line);
    fclose(state->current_file);

    state->current_file = stdin;
    printf("Finished reading %s\n", path);

  }
}
