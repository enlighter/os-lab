/*######### CONTAINS NECESSARY FUNCTIONS FOR PARSING #########*/
/*######### PLEASE INCLUDE THE FILE "parse.h" IN THE SAME FOLDER #########*/
/*   Syntax:     command <|arg1> <|arg2> <|arg3> <|arg4> <|arg5>      */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

void init_commQ(commQ *comm)		//initialize structure instance
{
	int i=0;

	comm->isEmpty = 1;
	comm->currArg = 0;
	for(i = 0; i <= MAX_ARG_NUM; i++)
	{
		comm->command[i] = NULL;
	}
}

void free_commQ(commQ *comm)		//freeing memory when instance no longer required
{
	if(comm->isEmpty)
		return;
	else
	{
		for(; (comm->currArg) >= 0; (comm->currArg)--)
		{
			//printf("Freeing Q%d\n", comm->currArg);
			free(comm->command[comm->currArg]);
		}
	}
}

void insertToken(char *token, commQ *queue)			//insert elements to the commQ structure
{
	//char *temp = NULL;
	//printf("is empty? %d\n", queue->isEmpty);

	if(queue->isEmpty)
	{
		queue->isEmpty = 0;
		queue->currArg = 0;
		//printf("Was empty!\n");
	}
	else
	{
		(queue->currArg)++;
	}

	queue->command[queue->currArg] = strdup(token);

}

commQ parse(char *line)
{
	char token[MAX_LENGTH];
	commQ ret;
	int i=0;
	char *temp = NULL;

	init_commQ(&ret);
	ret.wait = 1;
	temp = strtok(line, TOKENIZER);
	//printf("temp: %s\n", temp);
	//insertToken(temp, &ret);

	if(temp == NULL)		//if there are no tokens at all
		return ret;

	for(i=1; i<=MAX_ARG_NUM && temp != NULL; i++)
	{
		//printf("temp: %s\n", temp);
		//printf("is empty? %d\n", ret.isEmpty);
		if(strcmp(temp, "&") != 0){
			insertToken(temp, &ret);
		}
		else{
			ret.wait = 0;
		}
		//printf("commQ: no.of arguments: %d\n", ret.currArg);
		temp = strtok(NULL, TOKENIZER);
	}

	return ret;
}

void print_commQ(commQ *read)			//for debugging purposes
{
	if(read->isEmpty)
	{
		printf("Read structure empty!\n");
		return;
	}
	
	int i=0;

	printf("The current parsed struct: \n");
	for(i=0; i<= read->currArg; i++)
	{
		printf("%s", read->command[i]);
	}
	printf("\n");
}

/*
int main()			//for testing purposes
{
	commQ comm;
	char line[MAX_LENGTH];
	int i=0;

	init_commQ(&comm);
	//printf("Is empty? %d\n", comm.isEmpty);

	printf("Command input:\n");
	if (!fgets(line, MAX_LENGTH, stdin))
	{
		printf("read error!\n");
	}

	printf("line : %s ",line);

	comm = parse(line);
	printf("arguments : %d ", comm.currArg);
	i = 0;

	while(!comm.isEmpty && i<=comm.currArg)
	{
		printf("%s\n", comm.command[i]);
		i++;
	}

	free_commQ(&comm);

	return 0;
}
*/
