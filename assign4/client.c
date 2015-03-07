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

    int main(int argc, char *argv[]){
    int  fd = 0, sockd, rc, *p, sl;
    FILE *fp;
    struct sockaddr_un uxadr;
    struct msghdr mh;
    struct cmsghdr *cmp;
    char ctrl[sizeof(struct cmsghdr)+10];
    struct iovec iov[1];
    char eab[100],fname[100];
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
    //khoi tao socket
    uxadr.sun_family = AF_UNIX;
    strcpy(uxadr.sun_path, SOCKNAME);

    sockd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockd<0){
        perror("socket");
        exit(1);
    }

    sl = sizeof(uxadr);
    rc = connect(sockd, (struct sockaddr *)&uxadr, sl);
    if(rc<0){
        perror("connect");
        exit(2);
    }

    if(argc>=2){ // dua File name vao tu dong lenh
        sprintf(fname,"%s",argv[1]);
    }
    else{// neu chua co File name thi bat dau nhap file name vao
        printf("Bitte Filename eingeben\n");
        fflush(stdin);
        gets(fname); 

    }

    iov[0].iov_base = fname;
    iov[0].iov_len = strlen(fname)+1;
    rc = sendmsg(sockd, &mh, 0); // guoi filename sang server
    if(rc<0){
        perror("sendmsg");
        exit(3);
    }

    iov[0].iov_base = eab;
    iov[0].iov_len = sizeof(eab);
    rc = recvmsg(sockd, &mh, 0); // nhan thong bao va File descriptor tu server
    cmp = CMSG_FIRSTHDR(&mh);
    p = (int *)CMSG_DATA(cmp);
    if(rc<0){
        perror("recvmsg");
        exit(4);
    }


    switch(eab[0]){// kiem tra thong bao
        case 1:// file khong ton tai
            printf("File ist nicht existiert!\n");
            break;
        case 2:// file ton tai
            printf("File ist existiert. Filedeskriptor ist bereits zu verwandel!\n");


fd = *p; //fault here with GDB debug


            printf("fd=%d\n",fd);
            fp = fdopen(fd,"r");
            if(fp=NULL)
                printf("fehler fd\n");
            printf("Fileinhalt ausgeben\n");
            printf("=============================================\n");
            while(fread(eab,100,1,fp)>0)
                printf("%s", eab);
            printf("=============================================\n");
            fclose(fp);
            break;
    }
    close(sockd);
    }