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

int main(int argc,char *argv[]){
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