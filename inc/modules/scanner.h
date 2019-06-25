#ifndef __SCANNER_MODULE_
#define __SCANNER_MODULE_ 1

#define __SCANNER_MODULE_VERSION 1

#include "common.h"

typedef struct scanner_module scanner_module;

struct scanner_module {
  uint8_t version;
  BOOLEAN initialized;

  void (*initialize)(struct scanner_module *);
};

#endif