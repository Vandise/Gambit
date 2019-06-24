#include "modules/error.h"

static const char *error_messages[] = {
  "No error"
};

static void initialize(error_module *self) {
  for(int i = 0; i < __ERROR_MSG_SIZE; i++) {
    self->messages[i] = error_messages[i];
    self->error_list_size++;
  }

  self->initialized = TRUE;
}

error_module ErrorModule = {
  .version = __ERROR_MODULE_VERSION,
  .error_list_size = 0,
  .initialized = FALSE,
  .messages = NULL,

  .initialize = initialize,
};

// module interface

void* load_module() {
  initialize(&ErrorModule);
  return &ErrorModule;
}