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
	char * argv[4];
	argv[0] = "child.o";
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
			if(execv(argv[0],argv) == -1){
				printf("Error in excecuting child process\n");
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
}	

int child(int argc,char *argv[]){
	char msg[BUFF];
	int n, i;
	time_t t; 
	srand((unsigned) time(&t));

	int temp = atoi(argv[1]);					
	int fd = atoi(argv[2]);

	int n3;
	read(fd, msg, BUFF);
	sscanf(msg, "%d", &n3);

	char txt_file[BUFF];
	sprintf(txt_file,"%s%d", "data", n3);

	FILE *in = fopen(strcat(txt_file, ".txt"), "r");
	int input_num[5];
	fscanf(in,"%d %d %d %d %d", &input_num[0], &input_num[1], &input_num[2], &input_num[3], &input_num[4]);
	
	int fd1 = atoi(argv[3]);
	memset(&msg[0], 0, sizeof(msg));
	sprintf(msg,"%d", READY);
	write(fd1, msg, BUFF);

	int n1, n2;
	int count = 0, num_n = 0;
	while(1){
		memset(&msg[0], 0, sizeof(msg));
		read(fd,msg,BUFF);
		sscanf(msg,"%d",&n);

		if(n == REQUEST){					
			num_n = 0;
			for(i=0; i<5; i++)
			{
				if(input_num[i] > 0)
					num_n++;
			}
			if(num_n == 0){
				sprintf(msg,"%d",-1);
				write(fd1, msg, BUFF);
			}
			else{
				int rnd_n = rand()%num_n;
				for(i=0; i<5; i++){
					if(input_num[i] > 0 && rnd_n==0){
						sprintf(msg,"%d",input_num[i]);
						write(fd1, msg, BUFF);
						break;
					}
					else if(input_num[i] > 0)
						rnd_n--;
				}
			}
		}

		else if(n == PIVOT){
			read(fd, msg, BUFF);
			sscanf(msg,"%d", &n1);

			count = 0;
			for(i=0; i<5; i++){
				if(input_num[i]>n1)
					count++;
			}

			memset(&msg[0], 0, sizeof(msg));
			sprintf(msg,"%d", count);
			write(fd1, msg, BUFF);
			
			read(fd, msg, BUFF);
			sscanf(msg, "%d", &n2);

			if(n2 == SMALL){
				for(i=0; i<5; i++){
					if(input_num[i] < n1)
						input_num[i] = -1;
				}
			}
			if(n2 == LARGE){
				for(i=0; i<5; i++){
					if(input_num[i] > n1)
						input_num[i] = -1;
				}
			}
			if(n2 == KILL){
				break;
			}
		}
	}
	fclose(in);
}
