#ifndef __ERROR_MODULE_
#define __ERROR_MODULE_ 1

#define __ERROR_MODULE_VERSION 1
#define __ERROR_MSG_SIZE 1
#define __ERROR_MODULE_NAME "error"

#include "common.h"

typedef struct error_module error_module;

struct error_module {
  uint8_t version;
  uint8_t error_list_size;
  BOOLEAN initialized;
  const char *(messages)[__ERROR_MSG_SIZE];

  void (*initialize)(struct error_module *);
};

//extern error_module ErrorModule;

#endif