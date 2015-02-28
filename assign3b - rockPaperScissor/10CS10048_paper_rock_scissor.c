#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>	/* Include this file to use pipes */

#define	SUCCESS	0
#define FAULT	-1
#define BUFSIZE 80	//write lines of 80 chars into the pipe.

int participant();	//to be executed by a child for participating in the game
int mediator();		//to be used by master(host) to mediate the game 
inline void printChildReturn(int *, int *, int *, int *); //to signal child end

int master()		//host of the game
{
	int fdc[2], fdd[2];		//pipes for communicating with child c & d respectively
	char line[BUFSIZE];
	int pidC = FAULT, pidD = FAULT;	//process ids of children C & D respectively
	int wpid = 0, status = -100;	//pid of a child ended
	int ret = SUCCESS;				//return value

	pipe(fdc);
	pipe(fdd);
	printf("Master P: pipes created\n");

	pidC = fork();		//fork child C
	if(!pidC)			//C executing
	{
		printf("C is born! [%d]\n", getpid());

		close(fdc[0]); /* The child will not read and
				     hence we close fdc[0] */
	}
	else		//master executing
	{
		printf("C created : %d\n", pidC);

		close(fdc[1]); /* The parent will not write and
				     hence we close fdc[1] */

		pidD = fork();		//fork child D
		if(!pidD)			//D executing
		{
			printf("D is born! [%d]\n", getpid());

			close(fdd[0]); /* The child will not read and
				     hence we close fdd[0] */
		}
		else		//master executing
		{
			printf("D created : %d\n", pidD);

			close(fdd[1]); /* The parent will not write and
				     hence we close fdd[1] */
		}
	}

	while(1)
	{
		if(!pidC)		//C executing
		{
			printf("C playing game: %d\n", getpid());
		}
		else if(!pidD)	//D executing
		{
			printf("D playing game: %d\n", getpid());
		}
		else			//P (master) executing
		{
			printf("Master hosting the game: %d\n", getpid());

			wpid = wait(&status);
			printChildReturn(&wpid, &status, &pidC, &pidD);
			if(status != 0)
				ret = FAULT;

			wpid = wait(&status);
			printChildReturn(&wpid, &status, &pidC, &pidD);
			if(status != 0)
				ret = FAULT;

			return ret;			

		}
	}
}

int main()
{
	printf("Welcome to the game Rock-Paper-Scissors! [%d]\n", getpid());

	return (master());
}

int participant()
{

}

int mediator()
{

}

inline void printChildReturn(int *wid, int *stat, int *idC, int *idD)
{
	if(*wid == *idC)
		printf("C ended with status %d\n", *stat);
	else if(*wid == *idD)
		printf("D ended with status %d\n", *stat);
}