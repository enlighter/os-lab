// The maximum number of customers
#define MAX_CUSTOMERS 25

// Define # of semaphores.
#define NSEMS 4

// waitingRoom Limits the # of customers allowed
// to enter the waiting room at one time.
#define WAITINGROOM 0

// barberChair ensures mutually exclusive access to
// the barber chair.
#define BARBERCHAIR 1

// barberPillow is used to allow the barber to sleep
// until a customer arrives.
#define BARBERPILLOW 2

// seatBelt is used to make the customer to wait until
// the barber is done cutting his/her hair.
#define SEATBELT 3

//Shared Memory
#define PERMS   0600