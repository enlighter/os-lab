#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>

#define SOCKNAME "sockunix"
bool end = false;
void sigfkt(int signr){
    printf("SIGINT empfangen ...\n");
    end = true;
}