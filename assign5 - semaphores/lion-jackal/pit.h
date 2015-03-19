/* Main include header for the whole lion-jackal problem */
#include <sys/types.h>	//for 'key_t'

#define	FAULT 			-1	/* return value of a function on faulty execution */
#define	SUCCESS			0	/* return value of a function on successful execution */
#define	PIT_CAPACITY	50	/* maximum pit capacity */
#define FILL_VALUE		10	/* the amount deposited by ranger in 1 go */
#define NO_OF_PITS		3
/* We have to have 2 semaphores for each pit, one for controlling access and
the other for actual food value 
0  1  2  3  4  5
W  F  W  F  W  F
The even (2*i) semaphores are for access control (mutex) for (i+1)th pit
The odd (2*i + 1) semphores are the meat values for (i+1)th pit*/
#define SEMAPHORE_SIZE	NO_OF_PITS*2
#define	MAX_TRIES		500
#define MAX_INSTANCES	10	/* maximum no. of instances of a type to be spawned */
#define EXECUTION_TIME	1	/* total amount to time to run the forest */

typedef struct sembuf sBuf;

sBuf food;	//sembuf variable to refill pit with food
sBuf waitNSignal;	//sembuf variable to request and leave pit access

int getKey(key_t *, int *);	//get a free semaphore key
int instantiate(char *, int);	//fork the necessary processes
int be_a_lion(key_t *);		//main method for a lion process
int be_a_jackal(key_t *);		//main method for a jackal process
int be_a_ranger(key_t *);		//main method for a ranger process
int randomlyChoosePit();	//Generate random number to choose a pit
int printPitStatus(int);	//semctl(GETVAL)

void signalHandler(int);	//To handle manual termination of process

extern inline int getPitValue(int, int);	//get the meat value of a particular pit
extern inline int getStatusValue(int, int);	//get the status value of a particular pit