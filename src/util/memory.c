#include "util/memory.h"
#include "common.h"

List memory_list;
BOOLEAN list_initialized = FALSE;

void* malloc_trace(size_t size, const char *file, int line, const char *func) {
  void *p = malloc(size);
  if (PROJECT_MEMORY_DEBUG) {
    memory_list_init();

    MemoryItem *itm = malloc(sizeof(MemoryItem));
    itm->file = file; itm->line = line; itm->func = func;
    itm->pp = p;
    list_ins_next(&memory_list, NULL, itm);

    #if PROJECT_MEMORY_DEBUG_VERBOSE == 1
      printf ("Allocated = %s, %i, %s, %p[%li]\n", file, line, func, p, size);
    #endif
  }
  return p;
}

void free_trace(void *p, const char *file, int line, const char *func) {
  if (PROJECT_MEMORY_DEBUG) {
    memory_list_init();
    #if PROJECT_MEMORY_DEBUG_VERBOSE == 1
      printf ("Free = %s, %i, %s, %p\n", file, line, func, p);
    #endif
    remove_memory_list_item(p);
  }
  free(p);
  p = NULL;
}

void debug_list_destroy(void *pp) {
  __FREE__(pp);
}

void remove_memory_list_item(void *p) {
  ListElmt *element = list_head(&memory_list);
  MemoryItem *itm = list_data(element);

  // head element
  if (itm->pp == p) {
    list_rem_next(&memory_list, NULL, (void **)&itm->pp);
    free(itm);
    return;
  }

  while (1) {
    itm = element->next->data;
    if (itm->pp == p) {
      list_rem_next(&memory_list, element, (void **)&itm->pp);
      free(itm);
      break;
    }

    if (list_is_tail(element))
      break;
    else
      element = list_next(element);
  }
}

static void memory_list_init() {
  if (!list_initialized) {
    list_init(&memory_list, free);
    list_initialized = TRUE;
  }
}

void print_memory_summary() {
  #if PROJECT_MEMORY_DEBUG_VERBOSE == 1
    printf ("Memory List: Objects allocated - %d\n", list_size(&memory_list));
  #endif

  if (list_size(&memory_list) == 0) {
    return;
  }

  ListElmt *element = list_head(&memory_list);
  MemoryItem *itm = list_data(element);

  while (1) {

    #if PROJECT_MEMORY_DEBUG_VERBOSE == 1
      printf ("Leaked Memory Item: %s, %i, %s, %p\n", itm->file, itm->line, itm->func, itm->pp);
    #endif

    if (list_is_tail(element))
      break;
    else
      element = list_next(element);
  }
}