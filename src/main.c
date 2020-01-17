#include <dlfcn.h>
#include "common.h"
#include "modules/scanner/shared.h"

char* get_file_path(char* s) {
  int size = strlen(s);

  for (int i = size - 1; i > 0; i--) {
    if (s[i] == '/') {
      break;
    }
    s[i] = '\0';
  }

  return s;
}

int main(int argc, const char* argv[]) {
  char path[strlen(argv[0]) + 255];
  char *pathp = path;

  realpath(argv[0], pathp);

  get_file_path(pathp);
  strcat(path, "../lib/scanner.so");

  printf("\n %s \n", pathp);

  struct scanner_module *m;
  void *handle = dlopen(pathp, RTLD_NOW);
  m = dlsym(handle, "ScannerModule");

  printf("\n\t%d\n", m->version);

  Scanner* scanner = m->init(".tmp/gt/exists.g");

  return 0;
}
