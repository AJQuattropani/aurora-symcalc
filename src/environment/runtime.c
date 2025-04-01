#include "runtime.h"

void run_command_environment() {
  // main loop
  struct environment env = initialize_environment();

  /*char *line;
  size_t size;
  struct stringview_list views = new_list(4);
  for (ssize_t read = getline(&line, &size, env.current_file); read >= 0;
       read = getline(&line, &size, env.current_file)) {

    // push first character to arg[0]
    push_view(&views, line);
    line[strcspn(line, "\n")] = '\0'; // remove endline char
    // handling , delimiter directly
    for (size_t i = 0; i < size; i++) {
      int check = (line[i] == ' ');
      if (check) {
        push_view(&views, line + i + 1); // safe since buffer is null-terminated
        line[i] = '\0';
      }
    }

    const struct symbol *current_symbol = get_value(&env.symbol_map, line);
    if (current_symbol != NULL) {
      execute_from_symbol(&env, current_symbol, views.data, views.size);
    } else {
      fprintf(stderr, "Unknown command.\n");
    }
    // check status of environment
    if (ESC == env.status) {
      printf("Exiting program.\n");
      current_symbol = NULL;
      break;
    }

    empty_stringlist(&views); // this buffer can only get larger over
    // the program's lifetime.
  }
  free_stringlist(&views);
  free(line);
  */
  read_with_state(&env);

  empty_symbolmap(&env.symbol_map); // clear symbolmap
  
  printf("\nFinished running program.\n");
}
