#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//#include <pwd.h>
//#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
//#include <sys/stat.h>

#define MAX_LENGTH  1024
#define CD          1
#define PWD         2
#define MKDIR       3
#define RMDIR       4
#define LS          5
#define CP          6
#define EXIT        -1
#define NO_SUCH_BUILTIN 0

int isBuiltIncommand(char *, int *);
void executeBuiltIncommand(int);

int main(int argc, char *argv[]) 
{
  char line[MAX_LENGTH], cwd[MAX_LENGTH];
  int command=0;

  /* */
  system("clear");
  printf("New custom shell (pid: %d) created:\n\n",getpid());

  while (1) 
  {

    printf("$ ");

    if(getcwd(cwd, sizeof(cwd)) != NULL)
    	fprintf(stdout, "%s >", cwd);
    else
    	perror("getcwd() error");

    if (!fgets(line, MAX_LENGTH, stdin)) break;
    //system(line);
    if (isBuiltIncommand(line, &command) != 0)
    {
      //printf("command = %d",command);
      executeBuiltIncommand(command);
    }
    else
    {
      //printf("Unknown command\n");
      system(line);
    }
  }

  return 0;
}

int isBuiltIncommand(char * cmd, int *comm)
{
  if( strncmp(cmd, "exit", strlen("exit")) == 0)
    *comm = EXIT;
  else if( strncmp(cmd, "pwd", strlen("pwd")) == 0)
    *comm = PWD;
  else if( strncmp(cmd, "ls", strlen("ls")) == 0)
    *comm = LS;
  else if( strncmp(cmd, "cd", strlen("cd")) == 0)
    *comm = CD;
  else if( strncmp(cmd, "cp", strlen("cp")) == 0)
    *comm = CP;
  else if( strncmp(cmd, "mkdir", strlen("mkdir")) == 0)
    *comm = MKDIR;
  else if( strncmp(cmd, "rmdir", strlen("rmdir")) == 0)
    *comm = RMDIR;
  else
    *comm = NO_SUCH_BUILTIN;

  return *comm;
}

void executeBuiltIncommand(int execute)
{
  switch(execute)
  {
    default:
    {
      printf("default\n");
      return;
    }
  }
}