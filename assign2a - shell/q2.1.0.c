#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>	

#define MAX_LENGTH 1024

int main(int argc, char *argv[]) 
{
  char line[MAX_LENGTH], cwd[MAX_LENGTH];

  /* */
  system("clear");
  printf("New custom shell created. Process id %d :\n\n",getpid());

  while (1) 
  {
    printf("$ ");

    if(getcwd(cwd, sizeof(cwd)) != NULL)
    	fprintf(stdout, "%s >", cwd);
    else
    	perror("getcwd() error");

    if (!fgets(line, MAX_LENGTH, stdin)) break;
    system(line);
  }

  return 0;
}