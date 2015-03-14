/* Welcome to the National Forest in Enlighter World!!
	Here you'll find lions and jackals
	and their caretaker, the Ranger!

	Oh, and don't forget to include "pit.h"
	how else are they gonna eat huh!!
--------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/* UNIX based systems' include headers to implement UNIX
standard semaphores	*/
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
/*-----------------*/
#include <semaphore.h>

#include "pit.h"	//Main include header for the whole lion-jackal problem

int main()
{
	short isLion = 0, isJackal = 0, isRanger = 0;	//bool values for which process to run
	int nJ = 0, nL = 0;		//no of. jackals and lions respectively
	int choice = 0;		//user choice selection value

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

	return SUCCESS;
}