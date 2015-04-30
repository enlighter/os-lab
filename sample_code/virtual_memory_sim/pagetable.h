#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <vmsim.h>

const static int pagesize = 4096;
const static int log_pagesize = 12;

typedef struct _pte {
  uint           vfn; /* Virtual frame number */
  uint           pfn; /* Physical frame number iff valid=1 */
  int           reference;
  bool_t        valid; /* True if in physmem, false otherwise */
  bool_t        modified;
} pte_t;

void pagetable_init();

/* Lookup the page table entry for the given virtual page.
 * If the page is not in memory, will have valid==0.
 * If the vfn has never been seen before, will create a new pte_t
 * with the given vfn and valid==0.
 * type is for statistical tracking.
 */
pte_t *pagetable_lookup_vaddr(uint vfn, ref_kind_t type);

void pagetable_test();

#endif /* PAGETABLE_H */
