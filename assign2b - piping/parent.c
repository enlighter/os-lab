#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFF 64
#define REQUEST 111
#define READY 222
#define SMALL 333
#define LARGE 444
#define KILL 555
#define PIVOT 666

int main(){
	int total_n = 25;
	int fd1[5][2];
	int fd2[5][2];
	int i,n,j;
	char msg[BUFF];
	char * argv[5];
	argv[0] = strdup("./child.out");
	time_t t;
	srand((unsigned) time(&t));
	for(i=0; i<5; i++){
		pipe(fd1[i]);  
		pipe(fd2[i]);
		if(fork() == 0){  

			// child process
			close(fd1[i][1]);
			close(fd2[i][0]);

			argv[1] = malloc(BUFF);
			argv[2] = malloc(BUFF);
			argv[3] = malloc(BUFF);

			sprintf(argv[1],"%d",0);
			sprintf(argv[2],"%d",fd1[i][0]);
			sprintf(argv[3],"%d",fd2[i][1]);
			argv[4] = NULL;
			if(execvp(argv[0],argv) == -1){
				perror("child execution: ");
				break;
			}
		}
		else
		{ 
			// parent process
			close(fd1[i][0]);
			close(fd2[i][1]);				

			memset(&msg[0], 0, sizeof(msg));
			sprintf(msg,"%d", i+1);
			write(fd1[i][1], msg, BUFF);
			read(fd2[i][0], msg, BUFF);		
			sscanf(msg,"%d",&n);	
			if(n != READY){
				printf("Error in Child process :  %d\n",n);
			}
			printf("Child [%d] sends READY\n",i+1);
		}
	}
	printf("Parent is READY\n");
	int k = total_n/2;

	while(1){
		char c;
		scanf("%c", &c);
		n = -1;
		do{
			int rnd_child = rand()%5;
			memset(&msg[0], 0, sizeof(msg));
			sprintf(msg,"%d", REQUEST);
			write(fd1[rnd_child][1], msg, BUFF);
			printf("Parent REQUESTs to Child %d\n",rnd_child+1);
			
			read(fd2[rnd_child][0], msg, BUFF);
			sscanf(msg,"%d",&n);
			printf("Child %d send %d to Parent\n",rnd_child+1,n);
		}while(n < 0);

		// Parent broadcasts PIVOT to all children
		int n1,total_gr = 0;
		printf("Parent broadcast pivot %d to all children\n",n);	
		for(i=0; i<5; i++){
			sprintf(msg,"%d",PIVOT);
			write(fd1[i][1],msg,BUFF);

			sprintf(msg,"%d",n);
			write(fd1[i][1],msg,BUFF);

			read(fd2[i][0],msg,BUFF);
			sscanf(msg,"%d",&n1);
			printf("Child %d receives pivot and replied %d\n",i+1,n1);
			total_gr += n1;
		}	
		printf("Total numbers greater than pivot:%d\n",total_gr);


		// Parent kills all children when Median found
		if(total_gr == k){
			printf("Median found: %d\n",n);
			
			for(i=0; i<5; i++){
				sprintf(msg,"%d", KILL);
				write(fd1[i][1], msg, BUFF);
				printf("Child %d terminates\n",i+1);
			}
			break;
		}

		// Parent sends SMALL / LARGE to respective children
		if(total_gr > k){		
			printf("Parent send SMALL to all children\n");
			for(i=0; i<5; i++){
				sprintf(msg,"%d", SMALL);
				write(fd1[i][1], msg, BUFF);
				printf("Child %d received SMALL signal and killed all elements smaller than %d\n",i+1,n);
			}
			printf("Value of k:%d\n",k);
		}
		if(total_gr < k){		
			printf("Parent send LARGE signal to all its children\n");
			for(i=0; i<5; i++){
				sprintf(msg,"%d", LARGE);
				write(fd1[i][1], msg, BUFF);
				printf("Child %d received LARGE signal and killed all elements greater than %d\n",i+1,n);
			}
			//Modify value of k
			k = k - total_gr;
			printf("Modified value of k:%d\n",k);
		}
	}

	free(argv[0]);
}	