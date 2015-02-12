/* including necessary headers */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>		
/*---------------*/

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
	
	i=1;
	proceed=1;
	/*---------------*/

	/* Loop to calculate and print fibonacci numbers */
	while(i<=n && proceed)
	{
		pid = fork();				//forking a new child process with each loop
		if(!pid)
		{
			//child process is executing
			proceed=0;
			printf("Child process executing: %d \t",getpid());
			//printf("i=%d, n=%d, fib=%d, pfib=%d, ppfib=%d\n",i,n,*fib,*pfib,*ppfib);

			/* Calculating current Fibonacci number */
			int j=0; 
			unsigned long fib=0, pre_fib=0;
			for(j = 1; j <= i; j++)
			{
				if (j<3)
				{
					fib=1;
					pre_fib = 1;
				}
				else
				{
					unsigned long temp = fib;
					fib = pre_fib + fib;
					pre_fib = temp;
				}
			}
			/*---------------*/
				
			printf("fib(%d) = %lu\n",i,fib);		// Printing current Fibonacci number


		}
		else if(pid>0)
		{
			proceed=1; 				// parent process is executing
		}
		i++;
	}
	/*---------------*/
	
	/* Parent process waiting for all child process to end */
	i = n;
	while(pid > 0 && i>0){
		wpid = wait(&status);
		printf("Child process ended: %d with status=%d\n\n",wpid,status);
		i--;
	}
	/*---------------*/
	return 0;
}
