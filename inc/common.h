#ifndef __PROJECT_COMMON__
#define __PROJECT_COMMON__ 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "util/memory.h"

#define PROJECT_MEMORY_DEBUG 1
#define PROJECT_MEMORY_DEBUG_VERBOSE 1
#define PROJECT_OUTPUT_VERBOSE 0

#define MAX_FILE_NAME_LENGTH    32
#define MAX_TOKEN_STRING_LENGTH 1024
#define MAX_SOURCE_LINE_LENGTH  1024

typedef enum {
    FALSE, TRUE,
} BOOLEAN;

#endif