#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char *read_file(const char *file_name) {
  const size_t buffer_size = 1024;

  errno = 0;
  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Error opening file: %s\n%s\n", file_name, strerror(errno));
    exit(EXIT_FAILURE);
  }

  char *my_file = malloc(buffer_size);

  size_t buffer_read = 0;
  int c;
  while ((c = fgetc(file)) != EOF) {
    if (buffer_read >= buffer_size) {
      my_file = (char *)realloc(my_file, buffer_read + 10);
      if (!my_file) {
        fprintf(stderr, "Memory allocation failed for file: %s\n", file_name);
        fclose(file);
        exit(EXIT_FAILURE);
      }
    }

    my_file[buffer_read++] = c;
  }

  my_file[buffer_read] = '\0'; // NULL

  fclose(file);
  return my_file;
}
