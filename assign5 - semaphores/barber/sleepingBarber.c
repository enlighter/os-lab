/* Solving sleeping barber problem using Semaphore
--------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

// The maximum number of customers
#define MAX_CUSTOMERS 25

// Define # of semaphores.
#define NSEMS 3

// WAITINGCHAIR gives te number of waiting customers
#define WAITINGCHAIR 0

// barber, the number of barbers (0 or 1) who are idle or waiting for customers
#define BARBER 1

#define MUTEX 2

//Shared Memory
#define PERMS   0600

// Flag to stop the barber thread when all customers have been serviced.
int allDone = 0; 
int numChairs = 0;
int semid;

int ipcinit(int nsems,int vals[])
{
	void *a;
	int i;

	if ((semid=semget(IPC_PRIVATE,nsems,PERMS|IPC_CREAT))<0) {
		perror("semget\n");
		return(1);
	}

	for (i=0; i<nsems; i++) {
		if (semctl(semid,i,SETVAL,vals[i])<0) {
			perror("setval:semctl\n");
			return(1);
		}
	}

	return(0);
}

int ipcend()
{
	int sts=0;

	if (semctl(semid,0,IPC_RMID)) {
		perror("semctl");
		sts=1;
	}
	return(sts);
}

int semleave(int snum)
{
	struct sembuf sop;

	sop.sem_num=snum;
	sop.sem_op=1;
	sop.sem_flg=0;
	if (semop(semid,&sop,1)) {
		perror("post:semop");
		//printf("-semleave err [%d]: %d\n",snum,semctl(semid,snum,GETVAL,0));
		return(-1);
	}
	//printf("-semleave [%d]: %d\n",snum,semctl(semid,snum,GETVAL,0));
	return(0);
}

int semuse(int snum)
{
	struct sembuf sop;

	sop.sem_num=snum;
	sop.sem_op=-1;
	sop.sem_flg=IPC_NOWAIT;
	if (semop(semid,&sop,1)) {
		//perror("wait:semop");
		//printf("+semuse err [%d]: %d\n",snum,semctl(semid,snum,GETVAL,0));
		return(-1);
	}
	//printf("+semuse [%d]: %d\n",snum,semctl(semid,snum,GETVAL,0));
	return(0);
}

void randwait(int secs) 
{
	int len;
		// Generate a random number...
	len = (int) ((drand48() * secs) + 1);
	sleep(len);
}

void customer(int num) 
{
	printf("Customer %d is going to the barber's shop.\n", num);
		// Take some random amount of time
	randwait(5);

	printf("Customer %d arrived at barber shop.\n", num);
	while(semuse(MUTEX) == -1);
	if(semctl(semid,WAITINGCHAIR,GETVAL,0) ==  numChairs && semctl(semid,BARBER,GETVAL,0) == 1){
		printf("Customer %d waking up barber.\n", num);
		semuse(BARBER);
		semleave(MUTEX);
	}
	else{
	// Seating at waiting chairs
		printf("Customer %d entering waiting room.\n", num);
		if(semuse(WAITINGCHAIR)==-1){
			printf("Customer %d leaving barber shop WITHOUT hair cut.\n", num);
			semleave(MUTEX);
			exit(0);
		}
		semleave(MUTEX);
		// Wait for the barber chair 
		while(semuse(BARBER) == -1){    

		}
		// customer leaves waiting chair
		semleave(WAITINGCHAIR);
		printf("Customer %d having haircut.\n", num);
	}
	
	//randwait(3);  

	// Give up the chair.
	//semleave(BARBER);
	while(semctl(semid,BARBER,GETVAL,0) == 0);
	printf("Customer %d leaving barber shop.\n", num);

	exit(0);
}

void theend(int s)
{
	allDone=1;
}

void barber() 
{
	while (allDone==0) {
	// Sleep until customer arrives
	//	printf("Barber is sleeping\n");
	//	semuse(BARBERPILLOW);

		if (semctl(semid,BARBER,GETVAL,0) == 0) {
			printf("Barber is cutting hair\n");

			// Take a random amount of time 
			randwait(3);
			printf("Barber has finished cutting hair.\n");

			// Release the customer 
			semleave(BARBER);
		}
		else {
			//printf("Barber is sleeping\n");
		}
	}
	exit(0);
}

int startbarber()
{ 
	int pid;

	pid=fork();
	switch(pid) {
		case 0:
			// Child
		if (signal(SIGUSR1,theend)<0) {
			perror("signal");
		}
		barber();
		case -1:
		perror("fork");
		return(-1);
		default:
			// Parent
		return(pid);
	}
}

int startcustomer(int i)
{
	int pid;

	pid=fork();
	switch(pid) {
		case 0:
			// Child
		customer(i);
		case -1:
		perror("fork");
		return(-1);
		default:
			// Parent
		return(pid);
	}
}


int main(int argc, char *argv[]) 
{
	int i, numCustomers;
	pid_t pid[MAX_CUSTOMERS],bpid;
	int initvalues[NSEMS];

	printf("\nEnter no. of Customers : ");
	scanf("%d",&numCustomers);
	printf("\nEnter no. of waiting chairs : ");
	scanf("%d",&numChairs);

	if (numCustomers > MAX_CUSTOMERS) {
		printf("The maximum number of Customers allowed is %d.\n", MAX_CUSTOMERS);
		exit(-1);
	}
		// Initialize random number generator
	srand48((long)getpid());


		// Setup initial values for semaphores 
	initvalues[WAITINGCHAIR]=numChairs;
	initvalues[BARBER]=1;
	initvalues[MUTEX]=1;

		// Create IPCs
	if (ipcinit(NSEMS,initvalues)) {
		exit(1);
	}

		// Create the barber process
	bpid=startbarber();

		// Create the customer processes.
	for (i=0; i<numCustomers; i++) {
		//printf("starting customers\n");
		pid[i]=startcustomer(i);
	}

		// Wait for customer processes to finish
	for (i=0; i<numCustomers; i++) {
		if (waitpid(pid[i],NULL,NULL)<0) {
			perror("waitpid");
		}
	}

		// Stop the barber thread.

		// Wake the barber (if sleeping)
		//semleave(BARBERPILLOW); 
	if (kill(bpid,SIGUSR1)<0) {
		perror("kill");
	}
	else{
		printf("Barber is done for the day!");
	} 

	if (waitpid(bpid,NULL,NULL)<0) {
		perror("waitpid");
	}

		// Remove IPCs
	ipcend();
	exit(0);
}
