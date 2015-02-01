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
#include "q2.1.0.h"


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
    if (processBuiltInCommand(line, &command) != 0)
    {
      //printf("command = %d",command);
      executeBuiltInCommand(command);
    }
    else
    {
      //printf("Unknown command\n");
      system(line);
    }
  }

  return 0;
}

int processBuiltInCommand(char * cmd, int *comm)
{
  if( strncmp(cmd, "exit", strlen("exit")) == 0){
    char * arg0 = strtok (cmd," ,.-");
    char * arg1 = strtok (NULL," ,.-");
    if(arg1 != NULL){
      printArgumentError();
      return 0;
      }
    return executeExitCommand();
    }
  else if( strncmp(cmd, "pwd", strlen("pwd")) == 0){
    char * arg0 = strtok (cmd," ,.-");
    char * arg1 = strtok (NULL," ,.-");
    if(arg1 != NULL){
      printArgumentError();
      return 0;
      }
      return executePwdCommand();
    }
  else if( strncmp(cmd, "ls", strlen("ls")) == 0){
    char * arg0 = strtok (cmd," ,.-");
    char * arg1 = strtok (NULL," ,.-");
    if(arg1 != NULL){
      printArgumentError();
      return 0;
      }
      return executeLsCommand();
    }
  else if( strncmp(cmd, "cd", strlen("cd")) == 0){
    char * arg0 = strtok (cmd," ,.-");
    char * arg1 = strtok (NULL," ,.-");
    char * arg2 = strtok (NULL," ,.-");
    if(arg2 != NULL){
      printArgumentError();
      return 0;
      }
      return executeCdCommand(arg1);
    }
  else if( strncmp(cmd, "cp", strlen("cp")) == 0){
    char * arg0 = strtok (cmd," ,.-");
    char * arg1 = strtok (NULL," ,.-");
    char * arg2 = strtok (NULL," ,.-");
    char * arg3 = strtok (NULL," ,.-");
    if(arg3 != NULL){
      printArgumentError();
      return 0;
      }
      return executeCpCommand(arg1,arg2);
    }
  else if( strncmp(cmd, "mkdir", strlen("mkdir")) == 0){
    char * arg0 = strtok (cmd," ,.-");
    char * arg1 = strtok (NULL," ,.-");
    char * arg2 = strtok (NULL," ,.-");
    if(arg2 != NULL){
      printArgumentError();
      return 0;
      }
      return executeMkdirCommand(arg1);
    }
  else if( strncmp(cmd, "rmdir", strlen("rmdir")) == 0){
    char * arg0 = strtok (cmd," ,.-");
    char * arg1 = strtok (NULL," ,.-");
    char * arg2 = strtok (NULL," ,.-");
    if(arg2 != NULL){
      printArgumentError();
      return 0;
      }
      return executeRmdirCommand(arg1);
    }
  else
    *comm = NO_SUCH_BUILTIN;

  return *comm;
}

void executeBuiltInCommand(int execute)
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

int executeCdCommand(char *arg)
{
  int status=0;

  arg = strcat("/", arg);
  status = chdir(arg);
  
  if(status!=0)
  {
    perror("Error:");
    return -1;
  }
  else
    return 0;
}

int executePwdCommand(){

  return 0;
}

int executeMkdirCommand(char * arg){

  return 0;
}

int executeRmdirCommand(char * arg){

  return 0;
}

int executeLsCommand(){

  return 0;
}

int executeCpCommand(char * arg1, char* arg2){

  return 0;
}

int executeExitCommand(){
  exit(0);
  return -1;
}

void printArgumentError(){
  printf("Argument Mismatch\n");
}
