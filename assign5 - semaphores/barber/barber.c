/* Welcome to the National Forest in Enlighter World!!
	Here you'll find lions and jackals
	and their caretaker, the Ranger!

	Oh, and don't forget to include "pit.h"
	how else are they gonna eat huh!!
--------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
/* UNIX based systems' include headers to implement UNIX
standard semaphores	*/
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
/*-----------------*/
#include <sys/stat.h>
#include <semaphore.h>

#include "barber.h"	//Main include header for the whole lion-jackal problem

static key_t semKey;
int childPid = FAULT;		//stores pid of child process last forked
static instanceID = 0;		//ID of the current process type

int getKey(key_t *candidate, int nsems)
{
	int i = 0;

	for(i = 1; i <= MAX_TRIES; i++)
	{
		if( (*candidate = semget((key_t) (i*10), nsems, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) ) == (key_t)FAULT )
		{
			perror("Couldn't get semaphore: ");
		}
		else
		{
			printf("Semaphore set allocated = %d\n", (int) *candidate );
			break;
		}
	}

	if(*candidate == (key_t)FAULT )
		return FAULT;
	else
		return SUCCESS;
}

int main()
{
	short isLion = 0, isJackal = 0, isRanger = 0;	//bool values for which process to run
	int nInstances = 1;	//J = 0, nL = 0;		//no of. jackals and lions respectively
	int choice = 0;		//user choice selection value
	char *mode = NULL;	//current mode

	printf("Welcome to the National Forest!!\n");

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
				printf("Invalid choice, please enter a valid choice!\n");
				choice = 0;
			}
		}
	}

	if( getKey(&semKey, NO_OF_PITS) == FAULT )	//get a semaphore array for required number of pits
	{
		printf("Unable to get semaphore, Exiting...\n");
		return FAULT;
	}

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

	if( instantiate(mode, nInstances) == FAULT )
	{
		printf("Something went wrong while populating the forest.\n");
	}

	/* Free all allocated variables that need to be explicitly freed */

	free(mode);

	if( semctl(semKey, 0, IPC_RMID, 0) == FAULT )
	{
		perror("Couldn't free semaphore: ");
	}

	/*-----Avoided memory leakage----------*/

	return SUCCESS;
}

int instantiate(char* type, int instances)
{
	for (instanceID = 1; instanceID <= instances && childPid != 0; ++instanceID)
	/* Only the main process should go into the loop, all child processes should skip it */
	{
		if( (childPid = fork()) < 0)
		{
			perror("fork error: ");
			return FAULT;
		}
	}

	if(!childPid)
	/* section to be executed by all child processes */
	{
		printf("Lion %d created! [pid:%d]", instanceID, getpid());

		if( strcpm(type,"lion") == 0)
		{
			/* This is a lion instance */
			be_a_lion();
		}
		else if( strcpm(type,"jackal") == 0)
		{
			/* This is a jackal instance */
			be_a_jackal();
		}
		else if( strcpm(type,"ranger") == 0)
		{
			/* This is a ranger instance */
			be_a_ranger();
		}
	}
}