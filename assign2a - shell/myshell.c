/*######### PLEASE INCLUDE THE FILE "myshell.h, parse.c & parse.h" IN THE SAME FOLDER #########*/

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
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include "myshell.h"
#include "parse.c"
/*--------NECESSARY HEADERS INCLUDED---------*/

commQ direction;      //global structure instance for storing parsed command

int main(int argc, char *argv[]) 
{
  char line[MAX_LENGTH], cwd[MAX_LENGTH];
  /* to store command line and to store current directory full path */
  char tempParsee[MAX_LENGTH];  //temporary sring to store the string to be parsed

  //int command=0;
  init_commQ(&direction);    //intialize structure

  /* Print the command prompt */
  system("clear");
  printf("New custom shell (pid: %d) created:\n\n",getpid());
  /*----------------------------*/

  /* Enter perpetual loop for main process of shell */
  while (1) 
  {

    printf("$ ");

    if(getcwd(cwd, sizeof(cwd)) != NULL)
    	fprintf(stdout, "%s >", cwd);
    else
    	perror("getcwd() error");

    if (!fgets(line, MAX_LENGTH, stdin)) break;
    //system(line);

    strcpy(tempParsee, line); //copy line to temp string to be parsed

    if (processBuiltInCommand(tempParsee) != NO_SUCH_BUILTIN)
    {
      printf("command = %s",line);
      //executeBuiltInCommand(command);
    }
    else
    {
      printf("PATH: %s\n", getenv("PATH"));
      system(line);
    }
  }
  /*----------------------------*/

  free_commQ(&direction);
  return 0;
}

int processBuiltInCommand(char * cmd)   //check for and process builtin commands
{

  //printf("command : %s", cmd);
  direction = parse(cmd);        //parsing the command line to get direction

  print_commQ(&direction);      //for debugging
  //printf("direction.currArg = %d\n", direction.currArg);


  if( strcmp(direction.command[0], "exit") == 0){
  /* check if main command is exit*/

    if(direction.currArg != 0){
    /* check if there are arguments in direction */

      printArgumentError();
      return -1;
      }

    return executeExitCommand();  //execute exit builtin when everything checks out

    }
  else if( strcmp(direction.command[0], "pwd") == 0){
  /* check if main command is pwd*/

    if(direction.currArg != 0){
    /* check if there are arguments in direction */

      printArgumentError();
      return -1;
      }

      return executePwdCommand(); //execute pwd builtin when everything checks out

    }
  else if( strcmp(direction.command[0], "ls") == 0){
  /* check if main command is ls*/

    if(direction.currArg != 0 ){
    /* check if there are arguments in direction */

      if(strcmp(direction.command[1], "-l") == 0 && direction.currArg == 1){
        return executeLsMinusLCommand();    //currently the only mode supported
      }

      printArgumentError();
      return -1;

      }

      return executeLsCommand();  //execute ls builtin when everything checks out

    }
  else if( strcmp(direction.command[0], "cd") == 0){
  /* check if main command is cd*/

    if(direction.currArg != 1){
    /* check if there are proper no. of arguments in direction */

      printArgumentError();
      return -1;
      }

      return executeCdCommand(direction.command[1]);  //execute cd builtin when everything checks out

    }
  else if( strcmp(direction.command[0], "cp") == 0){
  /* check if main command is cp*/

    if(direction.currArg != 2){
    /* check if there are proper no. of arguments in direction */

      printArgumentError();
      return -1;
      }

      return executeCpCommand(direction.command[1], direction.command[2]);  //execute cp builtin when everything checks out

    }
  else if( strcmp(direction.command[0], "mkdir") == 0){
  /* check if main command is mkdir*/

    if(direction.currArg == 0){
    /* check if there are proper no. of arguments in direction */

      printArgumentError();
      return -1;
      }

      int i=0;
      char *temp;

      for(i=1; i<=direction.currArg; i++)
      {
          temp = direction.command[i];

          if(!executeMkdirCommand(temp))  //try to execute mkdir builtin for all arguments
            continue;
          else
            return -1;    //the first instance mkdir fails, return with negative status
      }

      return 0;

    }
  else if( strcmp(direction.command[0], "rmdir") == 0){
  /* check if main command is rmdir */
    
    if(direction.currArg == 0){
    /* check if there are proper no. of arguments in direction */

      printArgumentError();
      return -1;
      }

      int i=0;
      char *temp;

      for(i=1; i<=direction.currArg; i++)
      {
          temp = direction.command[i];

          if(!executeRmdirCommand(temp))  //try to execute rmdir builtin for all arguments
            continue;
          else
            return -1;    //the first instance rmdir fails, return with negative status
      }

      return 0;

    }

  else
    return NO_SUCH_BUILTIN;

  return 0;
}

int executeCdCommand(char *arg)
{
  int status=0;
  char destination[MAX_LENGTH], currPos[MAX_LENGTH] ;

  if(getcwd(currPos, sizeof(currPos)) == NULL)
  {
      perror("getcwd() error");
  }
  strcpy(destination, currPos);
  strcat(destination, "/");
  strncat(destination, arg, strlen(arg)-1);
  status = chdir(destination);
  
  if(status!=0)
  {
    perror("Error");
    return -1;
  }
  else
    return 0;
}

int executePwdCommand(){
  char cwd[MAX_LENGTH];
  if(getcwd(cwd, sizeof(cwd)) != NULL)
      printf("Current Working Directory : %s\n", cwd);
    else{
      perror("getcwd() error");
      return -1;
    }
  return 0;
}

int executeMkdirCommand(char * arg){

  int ret=0;
  //char name[20];
  mode_t process_mask = umask(0);

  //strncpy(name, arg, strlen(arg));
  ret = mkdir(arg, S_IRWXU | S_IRWXG | S_IRWXO);

  if(!ret)
  {
    printf("Directory %s created\n",arg);
  }
  else
  {
    perror("Error");
  }

  umask(process_mask);
  return ret;
}

int executeRmdirCommand(char * arg){

  int ret=0;
  //char name[20];

  //strncpy(name, arg, strlen(arg));
  ret = rmdir(arg);

  if(!ret)
  {
    printf("Directory %s deleted\n",arg);
  }
  else
  {
    perror("Delete Error");
  }

  return ret;
}

int executeLsCommand(){
  char cwd[MAX_LENGTH];
  DIR *dp;
  dp=opendir(getcwd(cwd, sizeof(cwd)));

  struct dirent *sd;
  while((sd=readdir(dp))!=NULL)
  {
    printf("%s\t",sd->d_name);
  }
  printf("\n");
  closedir(dp);
  return 0;
}

int executeLsMinusLCommand(){
  char cwd[MAX_LENGTH];
  DIR *dp;
  dp=opendir(getcwd(cwd, sizeof(cwd))); 

  struct dirent *sd; 
  struct stat buf;
  struct passwd *p;
  struct group *g;
  struct tm *t;

  char time[26];
  char P[10]="rwxrwxrwx",AP[10]=" ";
  int i,j;
  while((sd=readdir(dp)) != NULL)
  {
    stat(sd->d_name,&buf);
  
    // File Type
    if(S_ISDIR(buf.st_mode))
      printf("d");
    else if(S_ISREG(buf.st_mode))
      printf("-");
    else if(S_ISCHR(buf.st_mode))
      printf("c");
    else if(S_ISBLK(buf.st_mode))
      printf("b");
    else if(S_ISLNK(buf.st_mode))
      printf("l");
    else if(S_ISFIFO(buf.st_mode))
      printf("p");
    else if(S_ISSOCK(buf.st_mode))
      printf("s");
    //File Permissions P-Full Permissions AP-Actual Permissions
    for(i=0,j=(1<<8);i<9;i++,j>>=1)
      AP[i]= (buf.st_mode & j ) ? P[i] : '-' ;
    printf("%s", AP);
    //No. of Hard Links

    printf("%5d", (int) buf.st_nlink);

    //User Name
    p=getpwuid(buf.st_uid);
    printf(" %.8s", p->pw_name);
    //Group Name
    g=getgrgid(buf.st_gid);
    printf(" %-8.8s", g->gr_name);
    //File Size

    printf(" %8d", (int) buf.st_size);

    //Date and Time of modification
    t=localtime(&buf.st_mtime);
    strftime(time,sizeof(time),"%b %d %H:%M",t);
    printf(" %s",time);
    //File Name
    printf(" %s\n",sd->d_name);
  }
  closedir(dp);
  return 0;
}

int executeCpCommand(char * arg1, char* arg2){
  char dest[MAX_LENGTH], currPos[MAX_LENGTH] ;
  if(getcwd(currPos, sizeof(currPos)) == NULL)
  {
      perror("getcwd() error");
  }
  strcpy(dest, currPos);
  strcat(dest, "/");
  strncat(dest, arg2, strlen(arg2)-1);

  struct stat attr1,attr2;
  stat(arg1, &attr1);
  stat(dest, &attr2);

  //printf("Last modified source time: %s", ctime(&attr1.st_ctime));
  //printf("Last modified target time: %s", ctime(&attr2.st_ctime));

  int timeDiff = difftime(attr2.st_ctime,attr1.st_ctime) > 0 ? 1 : -1;

  FILE *source,*target;
  char ch;
  if( access( arg1, F_OK ) == -1 ){
    printf("File missing !!! \n");
  }
  else if( access( arg1, R_OK ) == -1 ){
    printf("Read Permission, Access Denied !!! \n");
  }

  source=fopen(arg1,"r");
  target=fopen(dest,"w");
  if(source == NULL)
  {
    printf("Unable to open source file..ERROR in opening file!!\n");
    return -1;
  }

  if( target != NULL && access(  dest, W_OK ) == -1 ){
    printf("Write Permission, Access Denied !!! \n");
    return -1;
  }

  if( target != NULL && timeDiff == 1){
    printf("Error !!! Destination has a newer file !!! \n");
    return -1;
  }

  while((ch=fgetc(source))!=EOF)
  fputc(ch,target);/*writing to the target file*/
  printf("Copy Successful !!!\n");
  fclose(source);
  fclose(target);

  return 0;
}

int executeExitCommand(){
  exit(0);
  return -1;
}

void printArgumentError(){
  printf("Argument Mismatch\n");
}
