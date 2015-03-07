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
bool ende = false;
void sigfkt(int signr){
    printf("SIGINT empfangen ...\n");
    ende = true;
}

int main(){
int fd, sockd, sockd2, rc, *p, sl;
FILE *fp;
struct sockaddr_un uxadr;
struct msghdr mh;
struct cmsghdr *cmp;
char ctrl[sizeof(struct cmsghdr)+10];
struct iovec iov[1];
char eab[100];
//bat dau khai bao
mh.msg_name = NULL;
mh.msg_namelen = 0;
mh.msg_iov = iov;
mh.msg_iovlen = 1;
mh.msg_control = ctrl;
mh.msg_controllen = sizeof(ctrl);
mh.msg_flags = 0;
cmp = CMSG_FIRSTHDR(&mh);
cmp->cmsg_len = CMSG_LEN(fd);
cmp->cmsg_level = SOL_SOCKET;
cmp->cmsg_type = SCM_RIGHTS;
p = (int *)CMSG_DATA(cmp);
// khoi tao socket
uxadr.sun_family = AF_UNIX;
strcpy(uxadr.sun_path, SOCKNAME);

sockd = socket(AF_UNIX, SOCK_STREAM, 0);
if(sockd<0){
    perror("socket");
    exit(1);
}

unlink(uxadr.sun_path);
sl = sizeof(uxadr);
rc = bind(sockd, (struct sockaddr*)&uxadr, sl);
if(rc<0){
    perror("bind");
    exit(2);
}

rc = listen(sockd, 10);
if(rc<0){
    perror("listen");
    exit(3);
}

sigset(SIGINT, sigfkt);
printf("Warte auf Client-Anforderungen ....\n"); //cho client ket noi toi
do{ 
    sockd2 = accept(sockd, 0, 0);
    if(ende)
        break;
    if(sockd2<0){
        perror("accept");
        exit(4);
    }

    iov[0].iov_base = eab;
    iov[0].iov_len = sizeof(eab);
    rc = recvmsg(sockd2,&mh,0); //nhan File name tu client
    if(rc<0){
        perror("recvmsg");
        exit(5);
    }
    printf("%s\n",eab);
    if(rc>0){
        fp = fopen(eab,"r");// kiem tra xem file ton tai ko?
        if(fp==NULL){
            printf("file not existiert\n");
            eab[0]=1; // thong bao cho client file ko ton tai
        }
        else{
            fd = fileno(fp); // lay File descriptor
            eab[0]=2; // thong bao cho client file ton tai
            *p = fd; //gan File descriptor vao cau truc dieu khien
        }
    }
printf("fd=%d\n",*p);
    rc = sendmsg(sockd2,&mh,0); // guoi thong bao va File descriptor den client
    if(rc<0){
        perror("sendmsg");
    }
    close(sockd2);
}while(!ende);

close(sockd);
unlink(uxadr.sun_path);
}