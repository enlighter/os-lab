/* Welcome to the National Forest in Enlighter World!!
	Here you'll find lions and jackals
	and their caretaker, the Ranger!

	Oh, and don't forget to include "pit.h"
	how else are they gonna eat huh!!
	...and also "ranger.c" , "lion.c" & "jackal.c"
--------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>	/* contains permissions flags */
//#include <limits.h>
#include <errno.h>
#include <string.h>
#include <time.h>	/* randomize using time */
/* UNIX based systems' include headers to implement UNIX
standard semaphores	*/
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/mman.h>	/* header for shared memory creation */
/*-----------------*/
//#include <sys/stat.h>
//#include <semaphore.h>

#include "pit.h"	//Main include header for the whole lion-jackal problem
#include "lion.c"	//contains be_a_lion()
#include "jackal.c"	//contains be_a_jackal()
#include "ranger.c"	//contains be_a_ranger()

static key_t *semKey;
static int childPid = FAULT;		//stores pid of child process last forked
static short instanceID = 0;		//ID of the current process type

int printPitStatus(int semid)
{
	int i = 0, meat = FAULT, status = FAULT;

	printf("\n");
	for(i = 0; i < NO_OF_PITS; i++)
	{
		if( (meat = semctl(semid, 2*i + 1 , GETVAL, 0)) == FAULT )
		{
			perror("Semctl(GETVAL) : ");
			return FAULT;
		}

		if( (status = semctl(semid, 2*i , GETVAL, 0)) == FAULT )
		{
			perror("Semctl(GETVAL) : ");
			return FAULT;
		}

		printf("Pit %d has %d meat with status %d\n", i+1, meat, status);
	}
	printf("\n");

	return SUCCESS;
}

inline int getPitValue(int semid, int pit)	//get the meat value of pit "pit" withim semid
{
	int meat = FAULT;

	if( (meat = semctl(semid, 2*pit + 1 , GETVAL, 0)) == FAULT )
		perror("Semctl(GETVAL) : ");

	return meat;
}

inline int getStatusValue(int semid, int pit)	//get the status value of pit "pit" within semid
{
	int status = FAULT;

	if( (status = semctl(semid, 2*pit , GETVAL, 0)) == FAULT )
		perror("Semctl(GETVAL) : ");

	return status;
}

int getKey(key_t *candidate, int *semid)
{
	int i = 0;

	for(i = 1; i <= MAX_TRIES; i++)
	{
		/* We have to have 2 semaphores for each pit, one for controlling access and
		the other for actual food value */
		if( (*semid = semget((key_t) (i*10), SEMAPHORE_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) ) == FAULT )
		{
			perror("Couldn't get semaphore: ");
			return FAULT;
		}
		else
		{
			*candidate = (key_t) (i*10);
			printf("Semaphore set allocated = %d, key = %d\n", *semid, (int) *candidate );
			return SUCCESS;
		}
	}

	/*if(*semid == FAULT )
		return FAULT;
	else
		return SUCCESS;
	*/
}

int randomlyChoosePit()	//Generate random number to choose a pit
{
	double result = FAULT;	//choose the pit
	float factor = ((float) RAND_MAX + 1) / NO_OF_PITS;
	int choice = FAULT;
	int i=0;

	srand((unsigned int)time(NULL));	//seed randomization with current time each time

	for(i = 0; i < 10; i++){
	result = rand()/ factor;
    choice = (int)(result * 100.0);
    
    //printf("choice = %d\n", choice);
    choice = choice % NO_OF_PITS;
    //printf("choice = %d\n", choice);
	}

    return choice;
}

int main()
{
	short isLion = 0, isJackal = 0, isRanger = 0;	//bool values for which process to run
	int nInstances = 1;	/* no. of instances for each process type, only 1 instance of ranger type allowed */
	int choice = 0;		//user choice selection value
	int semID = 0;	//to store the semaphore set id
	char *mode = NULL;	//current mode
	int wpid = 0, status = 0;	//to capture the pid of an ending child
	int i = 0;

	/* creating shared memory for parent-child processes with fork() */
	semKey = mmap(NULL, sizeof *semKey, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	printf("Welcome to the National Forest!! [%d]\n", getpid());

	/* Take input of user choice */
	while(!choice)
	{
		printf("1. Lion\n2. Jackal\n3. Ranger\n");
		printf("Please choose which process would you like to run currently ? :");
		scanf("%d", &choice);

		switch(choice)
		{
			case 1:
			{
				isLion = 1;
				break;
			}
			case 2:
			{
				isJackal = 1;
				break;
			}
			case 3:
			{
				isRanger = 1;
				break;
			}
			default:
			{
				printf("\nInvalid choice, please enter a valid choice!\n");
				choice = 0;
			}
		}
	}
	/*---------------------------*/

	/* Request allocation of required semaphore set */
	if( getKey(semKey, &semID) == FAULT )	//get a semaphore array for required number of pits
	{
		printf("Unable to get semaphore, Exiting...\n");
		return FAULT;
	}
	/*---------------------------*/

	if(isLion)
	{
		mode = strdup("lion");
	}
	else if(isJackal)
	{
		mode = strdup("jackal");
	}
	else if(isRanger)
	{
		mode = strdup("ranger");
	}

	/* Initiate the semaphore set */
	for(i = 0; i < NO_OF_PITS; i++)
	{
		if( semctl(semID, 2*i , SETVAL, 1) == FAULT)
		/* Initializing all pits' access values to 1 */
		{
			perror("Semctl(SETVAL) : ");
			return FAULT;
		}

		if( semctl(semID, 2*i + 1, SETVAL, 0) == FAULT)
		/* Initializing all pits' meat values to 0 meat */
		{
			perror("Semctl(SETVAL) : ");
			return FAULT;
		}
	}

	if( printPitStatus(semID) == FAULT )
	{
		return FAULT;
	}
	/*---------------------------*/

	/* Tke user input about the no. of instances of process type */
	/* Ranger only has 1 instance */
	while(!isRanger)
	{
		printf("How many instances of %ss shall there be? :", mode);
		scanf("%d", &nInstances);

		if(nInstances > MAX_INSTANCES)
		{
			printf("Whoa! Not so many!! Let's be reasonable here and maybe select a number below 10?\n");
		}
		else if(nInstances <= 0)
		{
			printf("Cannot create %d no. of instances.\n", nInstances);
		}
		else
			break;
	}
	/*-----------------------------*/

	/* Creating the instances */
	if( instantiate(mode, nInstances) == FAULT )
	{
		printf("Something went wrong while populating the forest.\n");
	}
	/*-----------------------------*/

	/* Free all allocated variables that need to be explicitly freed */
	free(mode);

	if(childPid > 0)
	/* to be executed only by the parent having children */
	{
		/* Wait and capture the ending child processes */
		for(i=1; i <= nInstances; i++)
		{
			wpid = wait(&status);
			printf("-[%d] : %d\n", wpid, status);
		}

		/* remove allocated semaphore only after the child processes have ended */ 
		if( semctl(semID, 0, IPC_RMID, 0) == FAULT)
		{
			perror("Couldn't free semaphore: ");
		}

	}
	/*-----Avoided memory leakage----------*/

	return SUCCESS;
}

int instantiate(char* type, int instances)
{
	instanceID = 1;

	while ( instanceID <= instances && childPid != 0)
	/* Only the main process should go into the loop, all child processes should skip it */
	{
		printf("Creating child %d\n", instanceID);

		if( (childPid = fork()) < 0)
		{
			perror("fork error: ");
			return FAULT;
		}

		if(childPid > 0)	//to be executed only in parent
		{
			instanceID++;
		}

	}

	if ( childPid > 0 )
	/* section to be executed by the parent only */
	{
		//printf("instanceID = %d\n", instanceID);	//for debugging purposes
		--instanceID;
		//printf("instanceID = %d\n", instanceID);	//for debugging purposes
	}
	else
	/* section to be executed by all child processes */
	{
		printf("%d:\n", getpid());

		if( strcmp(type,"lion") == 0)
		{
			/* This is a lion instance */
			printf("Lion %d created! [pid:%d]\n", instanceID, getpid());
			return be_a_lion(semKey);
		}
		else if( strcmp(type,"jackal") == 0)
		{
			/* This is a jackal instance */
			printf("Jackal %d created! [pid:%d]\n", instanceID, getpid());
			return be_a_jackal(semKey);
		}
		else if( strcmp(type,"ranger") == 0)
		{
			/* This is a ranger instance */
			printf("Ranger %d created! [pid:%d]\n", instanceID, getpid());
			return be_a_ranger(semKey);
		}
	}

	return SUCCESS;
}