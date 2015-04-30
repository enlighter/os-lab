/*
 * fault.c - Defines the available fault handlers. One example,
 *           fault_random, is provided; be sure to add your handlers 
 	     to fault_handlers[].
 *
 */

#include <vmsim.h>
#include <fault.h>
#include <options.h>
#include <physmem.h>
#include <stdlib.h>
#include <stdio.h>

static void fault_random(pte_t *pte, ref_kind_t type);
static void fault_init_random();

fault_handler_info_t fault_handlers[2] = {
  { "random", fault_random },
  { NULL, NULL } /* last entry must always be NULL/NULL */
};

/* Initialize any state needed by the fault handlers here.
 * For example, the random number generator must be initialized for
 * fault_random. */
void fault_init() {
  fault_init_random();
}

void fault_init_random() {
  long seed = 1234567;
  srandom(seed);
}

void fault_random(pte_t *pte, ref_kind_t type) {
  int page;
  page = random() % opts.phys_pages;
  physmem_evict(page, type);
  physmem_load(page, pte, type);
}


