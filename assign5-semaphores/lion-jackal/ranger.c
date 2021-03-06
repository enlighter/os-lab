/* THE PRODUCER - RANGER (Ranger of the national forest) */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//#include <time.h>	/* randomize using time */
/* UNIX based systems' include headers to implement UNIX
standard semaphores	*/
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
//#include <sys/mman.h>
/*-----------------*/
#include "pit.h"

//extern short instanceID;	/* instanceID defined in nationalForest.c */

int be_a_ranger(key_t *sKey)		//main method for a ranger process
{
	
	int semid = 0;	//to store the semaphore set id
	int pitChoice = FAULT;
	
	int i=0, j=0, meat = FAULT, status = FAULT;
	
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
	waitNSignal.sem_flg = IPC_NOWAIT ;
	/* If unable to get access then it will try for the next pit,
	no waiting here */

	/* Default values for food */
	food.sem_op = FILL_VALUE;
	food.sem_flg = 0;
	/* If blocked, then process will be blocked,
	otherwise will continue */

	pitChoice = randomlyChoosePit();

    /* FOR TESTING PURPOSES *
    if( semctl(semid, 2*pitChoice , SETVAL, 0) == FAULT)
	// making pit[pitChoice]'s access value to 0
	{
		perror("Semctl(SETVAL) : ");
		return FAULT;
	}
    /*=======================*/

    i = 0;
    while(i < NO_OF_PITS)
    {

    	printf("Ranger requesting control over meat pit %d\n", pitChoice + 1);

    	meat = getPitValue(semid, pitChoice);
	
		/* REQUESTING CRITICAL SECTION (WAIT) */
		waitNSignal.sem_num = 2 * pitChoice;
		if( semop( semid, &waitNSignal, 1) == FAULT)
		{
			perror("semop(wait) : ");
			printf("Ranger denied access over meat pit %d\n", pitChoice + 1);
		}
		/*-----------------------------*/
		else if( meat < 0 || meat > PIT_CAPACITY - FILL_VALUE )
		{
			printf("Meat not within limits in pit %d, moving on...\n", pitChoice + 1);
		}
		else
		{
		/* Will enter critical section only when semop() above returns SUCCESS 
			and meat value is within limits */
			/* CRITICAL SECTION !! */
			printf("Ranger in control of meat pit %d\n", pitChoice + 1);
			food.sem_num = 2 * pitChoice + 1;
			if( semop( semid, &food, 1) == FAULT)
			{
				perror("semop(food) : ");
				return FAULT;
			}
			/*-----------------------------*/
			else
			{
				
				/* SIGNAL (After critical section) */
				waitNSignal.sem_op = 1;
				waitNSignal.sem_flg = 0;

				/* Send signal to waiting queues of all 3 pits */
				for( j = 1; j <= NO_OF_PITS; j++)
				{
					//printf("status of pit %d = %d\n", pitChoice + 1, getStatusValue( semid, pitChoice ) );

					status = getStatusValue( semid, pitChoice );

					if( status == FAULT)
					{
						printf("Sorry, the forest in on fire. Exiting...\n");
						return FAULT;
					}
					else if( status < 1 )
					/* Signal the wait queue of a pit only of the current status < 1
					otherwise the status will accumulate to a value > 1 */
					{
						if( semop( semid, &waitNSignal, 1) == FAULT)
						{
							perror("semop(signal) : ");
						}
					}

					//printf("status of pit %d = %d\n", pitChoice + 1, getStatusValue( semid, pitChoice ) );

					pitChoice = (pitChoice + 1) % NO_OF_PITS;
					waitNSignal.sem_num = 2*pitChoice;
				}

				/* for testing purposes *
				printPitStatus(semid);
				/*========================*/

				break;
			}
		}

		i++;	/* if unable to enter critical section, move on and try next pit */

		if(i == NO_OF_PITS - 1)
		/* Check if the last iteration is coming up */
		{
			//printf("Setting semaphore flag as 0\n");
			waitNSignal.sem_flg = 0;
			/* Wait on the last pit if all NO_OF_PITS pits are busy */
		}

		pitChoice = (pitChoice + 1) % NO_OF_PITS;
	}

	/* For debugging purposes */
	if( printPitStatus(semid) == FAULT)
	{
		return FAULT;
	}
}