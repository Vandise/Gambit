#ifndef __GAMBIT_COMMON__
#define __GAMBIT_COMMON__ 1

#include <math.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "util/memory.h"
#include "util/log.h"

#define GAMBIT_MEMORY_DEBUG 1
#define GAMBIT_MEMORY_DEBUG_VERBOSE 1
#define GAMBIT_OUTPUT_VERBOSE 0

typedef enum {
    FALSE, TRUE,
} BOOLEAN;

#endif