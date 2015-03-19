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

//extern short instanceID;	/* instanceID defined in nationalForest.c */

int be_a_ranger(key_t *sKey)		//main method for a ranger process
{
	double result = FAULT;	//choose the pit to fill
	int semid = 0;	//to store the semaphore set id
	int pitChoice = FAULT;
	int range = NO_OF_PITS;	//for range of random values
	float factor = ((float) RAND_MAX + 1) / range;
	//int i=0;
	
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

	/* Default values for waitNSignal */
	waitNSignal.sem_op = -1;
	waitNSignal.sem_flg = SEM_UNDO;

	/* Default values for food */
	food.sem_op = 10;
	food.sem_flg = 0;

	srand((unsigned int)time(NULL));	//see randomization with current time each time
	//for(i=0;i<=10;i++){	//for testing
	result = rand()/ factor; //% NO_OF_PITS) + 1;	/* choose randomly amongst the pit choices (1 - NO_OF_PITS) */
    pitChoice = (int)(result * 100.0);
    pitChoice = pitChoice % NO_OF_PITS;

    printf("Ranger requesting control over meat pit %d\n", pitChoice + 1);
	
	/* REQUESTING CRITICAL SECTION */
	waitNSignal.sem_num = 2 * pitChoice;
	if( semop( semid, &waitNSignal, 1) == FAULT)
	{
		perror("semop(waitNSignal) : ");
		return FAULT;
	}
	/*-----------------------------*/

	/* CRITICAL SECTION !! */
	food.sem_num = 2 * pitChoice + 1;
	if( semop( semid, &food, 1) == FAULT)
	{
		perror("semop(food) : ");
		return FAULT;
	}
	/*-----------------------------*/

	/* For debugging purposes */
	if( printPitStatus(semid) == FAULT)
	{
		return FAULT;
	}
}