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

int be_a_ranger(key_t *sKey)		//main method for a ranger process
{
	double result = FAULT;	//choose the pit to fill
	int pitChoice = FAULT;
	int range = NO_OF_PITS;	//for range of random values
	float factor = ((float) RAND_MAX + 1) / range;
	//int i=0;

	if( getKey(sKey, NO_OF_PITS) == FAULT )	//get the semaphore for pits
	{
		printf("Unable to get semaphore, Exiting...\n");
		return FAULT;
	}

	srand((unsigned int)time(NULL));	//see randomization with current time each time
	//for(i=0;i<=10;i++){	//for testing
	result = rand()/ factor; //% NO_OF_PITS) + 1;	/* choose randomly amongst the pit choices (1 - NO_OF_PITS) */
    pitChoice = (int)(result * 100.0);
    pitChoice = (pitChoice % NO_OF_PITS) + 1;

    printf("Ranger requesting control over meat pit %d\n", pitChoice);
	//}
}