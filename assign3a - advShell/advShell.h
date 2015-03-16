/* Please include parse.h file in the same directory */

#include "parse.h"

#define MAX_LENGTH  1024
#define CD          1
#define PWD         2
#define MKDIR       3
#define RMDIR       4
#define LS          5
#define CP          6
#define FAULT       -1
#define SUCCESS		0
#define NO_SUCH_BUILTIN -2

//#define 


int processBuiltInCommand(commQ *);	//check for and process builtin commands
//int processOtherCommands(char *);
void executeExecutable(char **, int);
int executeCdCommand(char *);
int executePwdCommand();
int executeMkdirCommand(char *);
int executeRmdirCommand(char *);
int executeLsCommand();
int executeLsMinusLCommand();
int executeCpCommand(char *, char*);
int executeExitCommand();
inline void printArgumentError();