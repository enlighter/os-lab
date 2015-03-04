/*######### HEADER FILE FOR "PARSE" INSTRUCTIONS (parse.c) #########*/
/*######### CONTAINS NECESSARY PREPROCESSOR AND FUNCTION DECLARATIONS #########*/

#define	MAX_ARG_NUM	5				//the maximum no. of arguments supported for a command
#define MAX_LENGTH	1024			//the maximum length of command-line
#define TOKENIZER	" \n"			//to tokenize a string for parsing

typedef struct command_queue		//struct to store parsed commands for a line in a linked-list
{
	/* data */
	short isEmpty;					//signifies if there is no command
	short wait;						//wait or not
	short outputRedirection;		//whether redirection of output to be done
	short inputRedirection;			//whether redirection of input to be done
	int currArg;					//no. of current arguments
	char *command[MAX_ARG_NUM+1];	//command[0]=command, rest are arguments for command
	char *redirectionArg;			//argument for I/O redirection

} commQ;

void init_commQ(commQ *);			//initialize a commQ instance
void free_commQ(commQ *);			//free dynamic memory from commQ instance that is no longer required
void print_commQ(commQ *);			//for debugging purposes
void insertToken(char *, commQ *);	//insert elements to the commQ structure
commQ parse(char *);				//to parse and store from the command line