#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char *read_file(const char *file_name) {
  errno = 0;
  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Error opening file: %s\n%s\n", file_name, strerror(errno));
    return NULL;
  }

  char *my_file = NULL;
  size_t buffer_size = 0;
  char line_buffer[256];

  while (fgets(line_buffer, sizeof(line_buffer), file)) {
    size_t line_length = strlen(line_buffer);
    my_file = realloc(my_file, buffer_size + line_length + 1);
    if (!my_file) {
      fprintf(stderr, "Memory allocation failed for file: %s\n", file_name);
      free(my_file);
      fclose(file);
      return NULL;
    }

    memcpy(my_file + buffer_size, line_buffer, line_length);
    buffer_size += line_length;
  }

  my_file[buffer_size] = '\0'; // NULL

  fclose(file);
  return my_file;
}
