//#include <config.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <vmsim.h>
#include <util.h>
#include <options.h>
#include <pagetable.h>
#include <physmem.h>
#include <stats.h>
#include <fault.h>

void init();
void test();
void simulate();

/* refs per '.' printed */
const static uint dot_interval = 100;
const static uint dots_per_line = 64;


int main(int argc, char **argv) {
	options_process(argc, argv);
	if (opts.test) {
    		test();
   	 	printf("Tests done.\n");
    		exit(0);
  	}
  
  	init();
  	simulate();
  	stats_output();
  
	return 0;
}

void init() {   
  pagetable_init();
  physmem_init();
  stats_init();
  fault_init();
}

void test() {
  printf("Running vmtrace tests...\n");
  util_test();
  stats_init();
  pagetable_test();
}

ref_kind_t get_type(char c)
{
	if (c == 'R') return REF_KIND_LOAD;
	if (c == 'W') return REF_KIND_STORE;
	return REF_KIND_CODE;
}


void simulate() {
  uint pid;
  char ch;
  vaddr_t vaddr;
  ref_kind_t type;
  pte_t *pte;
  fault_handler_t handler;
  uint count = 0;
  FILE *fin = NULL;	
  
  handler = opts.fault_handler->handler;
  
  if ((fin=fopen(opts.input_file, "r")) == NULL) {
	  fprintf(stderr, "\n Could not open input file %s.", opts.input_file);
	exit(1);
  }
   printf("\n Progress  ");
  while (fscanf(fin, "%d, %c, %x", &pid, &ch, &vaddr) !=EOF) {
	  type = get_type(ch);
	  stats_reference(type);
    
	  count++;
    if (opts.verbose && (count % dot_interval) == 0) {
      /* this process has no measurable effect on running time */
      printf(".");
      fflush(stdout); 
      if ((count % (dots_per_line * dot_interval)) == 0) { 
	      printf("\n");
	      fflush(stdout);
      }
    }
    
    pte = pagetable_lookup_vaddr(vaddr_to_vfn(vaddr), type);
    if (!pte->valid) {
      /* Fault */
      stats_miss(type);
      handler(pte, type);
    }
    
    pte->reference = 1;
    if (type == REF_KIND_STORE)
      pte->modified = TRUE;

    if (opts.limit && count >= opts.limit) {
      if (opts.verbose)
	printf("\nvmsim: reached %d references\n", count);
      break;
    }

  }
}

