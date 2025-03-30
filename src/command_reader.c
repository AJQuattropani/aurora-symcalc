#include "command_reader.h"

void read_file(const char *path) {

  if (path == NULL) {
    fprintf(stderr, "Empty string passed to run_file.");
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

  FILE *fptr;
  fptr = fopen(path, "r");
  if (fptr == NULL) {
    fprintf(stderr, "File not found: %s", path);
    exit(1);
  }

  char *line = NULL;
  size_t size = 0;
  for (ssize_t read = getline(&line, &size, fptr); read >= 0;
       read = getline(&line, &size, fptr)) {
    printf("%s\n", line);
  }

  free(line);
  fclose(fptr);

  printf("Succesfully read %s\n", path);
}

void read_from_in() {
  char *line = NULL;
  size_t size = 0;
  for (ssize_t read = getline(&line, &size, stdin); read >= 0;
       read = getline(&line, &size, stdin)) {
  line[strlen(line)-1] = '\0';
    if (0 == strncmp(line, "exit", 5))
      break;
    if (0 == strncmp(line, "open", 4))
      read_file(strrchr(line, ' ') + 1);
    printf("%s\n", line);
  }
  free(line);
}
