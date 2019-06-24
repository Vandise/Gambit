#include "gambit_lib.h"

static gambit_lib GambitModules[] = {

  ((gambit_lib){
    .version = 1,
    .name = __ERROR_MODULE_NAME,
    .instance = NULL
  })

};

static error_module* get_error_module(gambit_lib_module* self) {
  for (int i = 0; i < __GAMBITLIB_LIST_SIZE; i++) {
    if (!strcmp(__ERROR_MODULE_NAME, self->libraries[i].name)) {
      return self->libraries[i].instance;
    }
  }

  return NULL;
}

static void load_library(gambit_lib* lib) {
  char libpath[20];

  strcpy(libpath, "lib/");
  strcat(libpath, lib->name);
  strcat(libpath, ".so");

  void* handle = dlopen(libpath, RTLD_NOW);

  if (!handle) { printf("Unable to load Gambit Module: %s\n", libpath); exit(1); }

  load_lib_defn* load = (load_lib_defn*)dlsym(handle, __GAMBITLIB_LOAD_FN);

  if(!load) { printf("Unable to load Gambit Module. Missing function %s\n", __GAMBITLIB_LOAD_FN); exit(1); }

  lib->instance = load();
}

static void initialize(gambit_lib_module* self) {
  for(int i = 0; i < __GAMBITLIB_LIST_SIZE; i++) {
    self->libraries[i] = GambitModules[i];
    load_library( &(self->libraries[i]) );
  }

  self->initialized = TRUE;
}

gambit_lib_module GambitLibModule = {
  .lib_list_size = __GAMBITLIB_LIST_SIZE,
  .initialized = FALSE,

  .initialize = initialize,
  .get_error_module = get_error_module
};