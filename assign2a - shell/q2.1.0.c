/*######### PLEASE INCLUDE THE FILE "q2.1.0.h" IN THE SAME FOLDER #########*/

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
    if (processBuiltInCommand(line, &command) != NO_SUCH_BUILTIN)
    {
      //printf("command = %d",command);
      //executeBuiltInCommand(command);
    }
    else
    {
      printf("PATH: %s\n", getenv("PATH"));
      processOtherCommands();
      //system(line);
    }
  }

  return 0;
}

int processBuiltInCommand(char * cmd, int *comm)
{
  if( strncmp(cmd, "exit", strlen("exit")) == 0){
    char * arg0 = strtok (cmd," ");
    char * arg1 = strtok (NULL," ");
    if(arg1 != NULL){
      printArgumentError();
      return -1;
      }
    return executeExitCommand();
    }
  else if( strncmp(cmd, "pwd", strlen("pwd")) == 0){
    char * arg0 = strtok (cmd," ");
    char * arg1 = strtok (NULL," ");
    if(arg1 != NULL){
      printArgumentError();
      return -1;
      }
      return executePwdCommand();
    }
  else if( strncmp(cmd, "ls", strlen("ls")) == 0){
    char * arg0 = strtok (cmd," ");
    char * arg1 = strtok (NULL," ");
    char * arg2 = strtok (NULL," ");
    if(arg1 != NULL ){
      if(strncmp(arg1, "-l", strlen("-l")) == 0 ){
        return executeLsMinusLCommand();
      }
      printArgumentError();
      return -1;
      }

      return executeLsCommand();
    }
  else if( strncmp(cmd, "cd", strlen("cd")) == 0){
    char * arg0 = strtok (cmd," ");
    char * arg1 = strtok (NULL," ");
    char * arg2 = strtok (NULL," ");
    if(arg2 != NULL || arg1 == NULL){
      printArgumentError();
      return -1;
      }
      return executeCdCommand(arg1);
    }
  else if( strncmp(cmd, "cp", strlen("cp")) == 0){
    char * arg0 = strtok (cmd," ");
    char * arg1 = strtok (NULL," ");
    char * arg2 = strtok (NULL," ");
    char * arg3 = strtok (NULL," ");
    if(arg3 != NULL){
      printArgumentError();
      return -1;
      }
      return executeCpCommand(arg1,arg2);
    }
  else if( strncmp(cmd, "mkdir", strlen("mkdir")) == 0){
    char * arg0 = strtok (cmd," ");
    char * arg1 = strtok (NULL," ");
    char * arg2 = strtok (NULL," ");
    if(arg2 != NULL){
      printArgumentError();
      return -1;
      }
      return executeMkdirCommand(arg1);
    }
  else if( strncmp(cmd, "rmdir", strlen("rmdir")) == 0){
    char * arg0 = strtok (cmd," ");
    char * arg1 = strtok (NULL," ");
    char * arg2 = strtok (NULL," ");
    if(arg2 != NULL){
      printArgumentError();
      return -1;
      }
      return executeRmdirCommand(arg1);
    }
  else
    return NO_SUCH_BUILTIN;

  return 0;
}

int processOtherCommand(char *execute)
{
  
  return -1;
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
  char name[20];
  mode_t process_mask = umask(0);

  strncpy(name, arg, strlen(arg)-1);
  ret = mkdir(name, S_IRWXU | S_IRWXG | S_IRWXO);

  if(!ret)
  {
    printf("Directory %s created\n",name);
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
  char name[20];

  strncpy(name, arg, strlen(arg)-1);
  ret = rmdir(name);

  if(!ret)
  {
    printf("Directory %s deleted\n",name);
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
