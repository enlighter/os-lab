/* THE PRODUCER - RANGER (Ranger of the national forest) */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>	/* randomize using time */
/* UNIX based systems' include headers to implement UNIX
standard semaphores	*/
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/mman.h>
/*-----------------*/
#include "pit.h"

void be_a_ranger(key_t *sKey)		//main method for a ranger process
{
	int pitChoice = FAULT;	//choose the pit to fill

	srand((unsigned int)time(NULL));	//see randomization with current time each time
    pitChoice = (rand() % NO_OF_PITS) + 1;	/* choose randomly amongst the pit choices (1 - NO_OF_PITS) */

    printf("Ranger requesting control over meat pit %d\n", pitChoice);
}