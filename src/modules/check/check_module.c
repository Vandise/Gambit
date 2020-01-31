#include "modules/check/check_module.h"

static void print_header(const char* error) {
  printf("\n\033[36m-- %s -----------------------\033[0m\n", error);
}

static void print_error_message(const char* error_message) {
  printf("\n\033[0;31m\tError:\033[0m\n");

  printf("\n\t\t%s\n", error_message);
}

static void print_file_message_heading(const char* filename, unsigned int line_number) {
  printf("\n\033[36m\t%s %s %s %d:\033[0m\n", "In file", filename, "on line", line_number);
}

static void trace_error(
    const char* error,
    const char* message,
    const char* filename,
    char **suggestions,
    unsigned int line_number,
    unsigned int buffer_offset
) {

  int line_min = (line_number - 10) > 0 ? (line_number - 10) : 0;
  int line_max = (line_number + 10);
  int current_line = 0;

  print_header(error);
  print_error_message(message);

  if (FileModule.file_exists(filename)) {
    print_file_message_heading(filename, (line_number + 1));

    FILE *file = fopen(filename, "r");
    char line[1024];

    while (fgets(line, sizeof line, file) != NULL) {

      if (current_line >= line_min || current_line <= line_max) {
        printf("\t\t%04d: %s", (current_line + 1), line);

        if (current_line == line_number) {
          int errorpos = 0;
          printf("\033[0;31m\t\t      ");
          while (errorpos < buffer_offset) {
            printf("~");
            errorpos++;
          }
          printf("^\033[0m\n");
        }

      } else {
        break;
      }

      current_line++;
    }
    fclose(file);

  }
}

const struct check_module CheckModule = {
  .trace_error = trace_error
};