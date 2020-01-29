#include "util/file/file_module.h"

static BOOLEAN file_exists(const char* filename) {
  char buffer[strlen(filename) + 255];
  char* bufferp = buffer;

  realpath(filename, bufferp);

  FILE* file = fopen(bufferp, "r");
  if (file) {
    fclose(file);
    return TRUE;
  }
  return FALSE;
}

static BOOLEAN get_file_real_path(const char* s, char* buffer) {
  realpath(s, buffer);

  if (!file_exists(buffer)) {
    return FALSE;
  }

  int size = strlen(buffer);
  for (int i = size - 1; i > 0; i--) {
    if (buffer[i] == '/') {
      break;
    }
    buffer[i] = '\0';
  }

  return TRUE;
}

const struct file_module FileModule = {
  .get_file_real_path = get_file_real_path,
  .file_exists = file_exists
};