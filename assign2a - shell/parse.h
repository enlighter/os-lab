/*######### HEADER FILE FOR "PARSE" INSTRUCTIONS (parse.c) #########*/
/*######### CONTAINS NECESSARY PREPROCESSOR AND FUNCTION DECLARATIONS #########*/
#define	MAX_ARG_NUM	5				//the maximum no. of arguments supported for a command
#define MAX_LENGTH	1024			//the maximum length of command-line

typedef struct command_queue				//struct to store parsed commands for a line in a linked-list
{
	/* data */
	short isEmpty;							//signifies if there is no command
	int currArg;							//no. of current arguments
	char *command[MAX_ARG_NUM+1];			//command[0]=command, rest are arguments for command

} commQ;

void init_commQ(commQ *);
void free_commQ(commQ *);