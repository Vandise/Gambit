#include <dlfcn.h>
#include "common.h"
#include "modules/scanner/shared.h"

int main(int argc, const char* argv[]) {
  struct scanner_module *m;
  void *handle = dlopen("./lib/scanner.so", RTLD_NOW);
  m = dlsym(handle, "ScannerModule");

  printf("\n\t%d\n", m->version);

  Scanner* scanner = m->init(".tmp/gt/exists.g");

  return 0;
}
