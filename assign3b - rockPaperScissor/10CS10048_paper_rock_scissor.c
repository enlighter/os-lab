#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>	/* Include this file to use pipes */

#define	SUCCESS	0
#define FAULT	-1
#define BUFSIZE 80	//write lines of 80 chars into the pipe.

int child();

int master()
{
	int fdc[2], fdd[2];		//pipes for communicating with child c & d respectively
	char line[BUFSIZE];
	int pidC=FAULT, pidD=FAULT;		//process ids of children C & D respectively

	pipe(fdc);
	pipe(fdd);
	printf("Master P: pipes created\n");

	pidC = fork();		//fork child C
	if(!pidC)			//C executing
	{
		printf("C is born! [%d]\n", getpid());
	}
	else		//master executing
	{
		printf("C created : %d\n", pidC);

		pidD = fork();		//fork child D
		if(!pidD)			//D executing
		{
			printf("D is born! [%d]\n", getpid());
		}
		else		//master executing
		{
			printf("D created : %d\n", pidD);
		}
	}

	while(1)
	{
		if(!pidC)
		{
			printf("C playing game: %d\n", getpid());
		}
		else if(!pidD)
		{
			printf("D playing game: %d\n", getpid());
		}
		else
		{
			printf("Master hosting the game: %d\n", getpid());
		}
	}
}

int main()
{
	printf("Welcome to the game Rock-Paper-Scissors! [%d]\n", getpid());

	return (master());
}

int child()
{
	
}