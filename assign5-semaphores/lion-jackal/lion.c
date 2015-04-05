/* A CONSUMER - LION (Lion living in the national forest) */

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

int be_a_lion(key_t *sKey)		//main method for a lion process
{
	int semid = 0;	//to store the semaphore set id
	int pitChoice = FAULT;
	
	
	if( ( semid = semget(*sKey, SEMAPHORE_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) ) == FAULT )
	/* get the semaphore id for pits using the key to the pits */
	{
		perror("semget : ");
		return FAULT;
	}
	else
	{
		printf("Ranger : Semaphore set got = %d, key = %d\n", semid, (int) *sKey );
	}

	pitChoice = randomlyChoosePit();
}