/* including necessary headers */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
/*---------------*/		

#define BUFSIZE 80

int main()
{
	pid_t pid, wpid;					//pids of children processes
	int n=0, status=0, i=0;					//integer variables
	unsigned short proceed=0;				//binary flag for procession of loop

	/* Declaration and user variable input */
	printf("Fibchild - fibonacci series \n");
	printf("Give the length of fibonacci series to be printed: ");
	scanf("%d",&n);
	printf("\n");
	/*---------------*/

	/* Initializations */
	int arr[n+1];
	char buff[BUFSIZE];
	i=1;
	proceed=1;
	int p[2];
	pipe(p);
	/*---------------*/

	/* Loop to calculate and print fibonacci numbers */
	while(i<=n && proceed)
	{
		pid = fork();				//forking a new child process with each loop
		if(!pid)
		{
			//child process is executing
			proceed=0;
			close(p[0]); 	// The child will not read and hence we close fd[0]
			printf("Child process executing: %d \n",getpid());
			//printf("i=%d, n=%d, fib=%d, pfib=%d, ppfib=%d\n",i,n,*fib,*pfib,*ppfib);

			/* Calculating current Fibonacci number */
			int j, fib, pre_fib;
			for(j = 1; j <= i; j++)
			{
				if (j<3)
				{
					fib=1;
					pre_fib = 1;
				}
				else
				{
					int temp = fib;
					fib = pre_fib + fib;
					pre_fib = temp;
				}
			}
			/*---------------*/

			/* Sending current Fibonacci number through pipe*/

			sprintf(buff,"%d %d",i,fib);
			write(p[1],buff,BUFSIZE);
			/*---------------*/
			
		}
		else if(pid>0)
		{
			proceed=1; 			//parent process is executing 
		}
		i++;
	}
	/*---------------*/
	
	/* Parent process waiting for all child process to end and update the array */
	i = n;
	while(pid > 0 && i>0){
		sleep(1);
		int j,fib;
		read(p[0],buff,BUFSIZE);
		sscanf(buff,"%d %d",&j,&fib);
		//sscanf(buff,"%d",&fib);
		//printf("fib(%d) = %d\n",j,fib);
		arr[j]=fib;
		wpid = wait(&status);
		printf("Child process ended: %d with status=%d\n",wpid,status);
		i--;
	}
	/*---------------*/
	
	/* Parent process printing the array at end */
	if(pid > 0){
		int j;
		printf("The Fibonacci array : ");
		for(j = 1; j <= n; j++){
			printf("%d \t",arr[j]);
		}
		printf("\n");
	}
	/*---------------*/
	return 0;
}
