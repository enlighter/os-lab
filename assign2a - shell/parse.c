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
			free(comm->command[comm->currArg]);
		}
	}
}

/*
int main()			//for testing purposes
{
	commQ comm;
	init_commQ(&comm);
	free_commQ(&comm);

	return 0;
}
*/