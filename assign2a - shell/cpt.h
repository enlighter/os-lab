
#define CD          1
#define PWD         2
#define MKDIR       3
#define RMDIR       4
#define LS          5
#define CP          6
#define EXIT        -1
#define NO_SUCH_BUILTIN -2

int processBuiltInCommand(char *, int *);
//void executeBuiltInCommand(int);
int executeCdCommand(char *);
int executePwdCommand();
int executeMkdirCommand(char *);
int executeRmdirCommand(char *);
int executeLsCommand();
int executeLsMinusLCommand();
int executeCpCommand(char *, char*);
int executeExitCommand();
void printArgumentError();