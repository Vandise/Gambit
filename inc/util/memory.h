#ifndef __PROJECT_MEMORY_UTIL__
#define __PROJECT_MEMORY_UTIL__ 1

#include <stdlib.h>
#include <stdio.h>
#include "util/linked_list.h"

#define __MALLOC__(X) malloc_trace( X, __FILE__, __LINE__, __FUNCTION__)
#define __FREE__(X) free_trace( X, __FILE__, __LINE__, __FUNCTION__)

extern List memory_list;

typedef struct MemoryItem_ {
  const char *file;
  int line;
  const char *func;
  void *pp;
} MemoryItem;

void* malloc_trace(size_t size, const char *file, int line, const char *func);
void  free_trace(void *pp, const char *file, int line, const char *func);
void  debug_list_destroy(void *pp);

static void memory_list_init();
void print_memory_summary();
void remove_memory_list_item(void *p);

#endif