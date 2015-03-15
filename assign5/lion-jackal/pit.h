/* Main include header for the whole lion-jackal problem */

#define	FAULT 			-1	/* return value of a function on faulty execution */
#define	SUCCESS			0	/* return value of a function on successful execution */
#define	PIT_CAPACITY	50	/* maximum pit capacity */
#define NO_OF_PITS		3
#define	MAX_TRIES		500
#define MAX_INSTANCES	10	/* maximum no. of instances of a type to be spawned */

int getKey(key_t *, int);	//get a free semaphore key