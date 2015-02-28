#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>	/* Include this file to use pipes */
#include <time.h>	/* randomize using time */
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>	/* signaling between processes */


#define	SUCCESS	0
#define FAULT	-1
#define BUFSIZE 80	//write lines of 80 chars into the pipe.
#define PAPER 	1
#define SCISSOR 2
#define ROCK 	3
#define	MAXPOINT 10 //the maximum point at which to win

int fdc[2], fdd[2];		//pipes for communicating with child c & d respectively
char line[BUFSIZE];
int cPoints = 0, dPoints = 0;	//game points for the two participants


int participant();	//to be executed by a child for participating in the game
int mediate();		//to be used by master(host) to mediate the game 
inline void printChildReturn(int, int, int, int); //to signal child end
void childSigHandler (int);		// The signal handler for the child processes

int master()		//host of the game
{
	
	int pidC = FAULT, pidD = FAULT;	//process ids of children C & D respectively
	int wpid = 0, status = -100;	//pid of a child ended
	int ret = SUCCESS;				//return value
	int handC = 0, handD = 0;		//hands of the participants in one turn
	int mediator = -100;			//determines the game status

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

			signal(SIGTSTP, childSigHandler);           /* Register SIGTSTP handler */
      		signal(SIGCONT, childSigHandler);           /* Register SIGCONT handler */
      		signal(SIGUSR1, childSigHandler);           /* Register SIGUSR1 handler */

      		while (1) sleep(1);     /* Sleep until a signal is received from master */
		}
		else if(!pidD)	//D executing
		{
			printf("D playing game: %d\n", getpid());

			signal(SIGTSTP, childSigHandler);           /* Register SIGTSTP handler */
      		signal(SIGCONT, childSigHandler);           /* Register SIGCONT handler */
      		signal(SIGUSR1, childSigHandler);           /* Register SIGUSR1 handler */

      		while (1) sleep(1);     /* Sleep until a signal is received from master */
		}
		else			//P (master) executing
		{
			//printf("+++ Parent: Going to send signal SIGUSR%d to child\n", t);
      		kill(pidC, SIGTSTP);        /* Send stop signal to C */
      		kill(pidD, SIGTSTP);        /* Send stop signal to D */

			printf("Master hosting the game: %d\n", getpid());

			while(1)	//executing rounds
			{
				printf("------ Start of turn ------\n");

				/* Dealing with C */
				kill(pidC, SIGCONT);	//send ready signal to C

				printf("Master trying to read pipe from C\n");
				read(fdc[0], line, BUFSIZE);
				sscanf(line,"%d",&handC); /* Read the integer from the
						       line of characters read
						       from the pipe
					          */
				printf("Master reads: %d\n",handC);

				kill(pidC, SIGTSTP);	//send stop signal to C
				/*----------*/

				/* Dealing with D */
				kill(pidD, SIGCONT);	//send ready signal to D

				printf("Master trying to read pipe from D\n");
				read(fdd[0], line, BUFSIZE);
				sscanf(line,"%d",&handD); /* Read the integer from the
						       line of characters read
						       from the pipe
					          */
				printf("Master reads: %d\n",handD);

				kill(pidD, SIGTSTP);	//send stop signal to C	
				/*----------*/

				mediator = mediate(handC, handD);

				if(mediator == SUCCESS)
					break;

				printf("------ End of turn ------\n");
			}

			kill(pidC, SIGUSR1);		//sending user defined termination signal
			kill(pidD, SIGUSR1);		//sending user defined termination signal

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

int mediate(int hand1, int hand2)
{

}

void childSigHandler ( int sig )
{
   if (sig == SIGTSTP) {
      printf("+++ Child [%d] : Received signal SIGTSTP from master...\n", getpid());
      //sleep(1);
   } else if (sig == SIGCONT) {
      printf("+++ Child [%d] : Received signal SIGCONT from parent...\n", getpid());
      //sleep(5);
      participant();
   }
   else if( sig == SIGUSR1 )
   {
   		printf("+++ Child [%d] : Received signal SIGUSR1 from parent... exiting...\n", getpid());
   		exit(0);
   }
   //exit(0);
}

inline void printChildReturn(int wid, int stat, int idC, int idD)
{
	if(wid == idC)
		printf("C ended with status %d\n", stat);
	else if(wid == idD)
		printf("D ended with status %d\n", stat);
}