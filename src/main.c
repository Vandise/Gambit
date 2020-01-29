#include <dlfcn.h>
#include "common.h"
#include "modules/scanner/shared.h"
#include "util/file/file_module.h"

int main(int argc, const char* argv[]) {
  char path[strlen(argv[0]) + 255];
  char *pathp = path;

  printf("\n %d \n", FileModule.get_file_real_path(argv[0], pathp));
  printf("\n %s \n", pathp);

  printf("\n %d \n", FileModule.get_file_real_path(argv[1], pathp));
  printf("\n %s \n", pathp);

/*
  strcat(path, "../lib/scanner.so");
  struct scanner_module *m;
  void *handle = dlopen(pathp, RTLD_NOW);
  m = dlsym(handle, "ScannerModule");

  printf("\n%d\n", m->version);

  //Scanner* scanner = m->init(pathp);
*/

  return 0;
}
