#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>		/* including necessary headers */

int main()
{
	pid_t pid, wpid;						//pids of children processes
	int n=0, status=0, i=0;					//integer variables
	unsigned short proceed=0;				//binary flag for procession of loop
	unsigned long *fib, *pfib, *ppfib;				//fibonacci variables: <current>, <pre>, <pre-pre>
	//int *fp=NULL, *pfp=NULL, *ppfp=NULL;

	/*fp = &fib;
	pfp = &pfib;
	ppfp = &ppfib;

	/* Declaration and user variable input */
	printf("Fibchild - fibonacci series \n");
	printf("Give the length of fibonacci series to be printed: ");
	scanf("%d",&n);
	printf("\n");
	/*---------------*/

	/* Initializations */
	fib = mmap(NULL, sizeof *fib, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);	//creating shared memory for parent-child processes with fork()
	pfib = mmap(NULL, sizeof *pfib, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	ppfib = mmap(NULL, sizeof *ppfib, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	i=1;
	proceed=1;
	/*---------------*/

	/* Loop to calculate and print fibonacci numbers */
	while(i<=n && proceed)
	{
		pid = fork();				//forking a new child process with each loop
		proceed=0;
		if(pid>0)
			printf("Child process created: %d \n",pid);
		if(!pid)
		{
			//child process is executing
			printf("Child process executing: %d \n",getpid());
			//printf("i=%d, n=%d, fib=%d, pfib=%d, ppfib=%d\n",i,n,*fib,*pfib,*ppfib);

			/* Calculating current Fibonacci number */
			*ppfib = *pfib;
			*pfib = *fib;
			if (i<3)
			{
				*fib=1;
			}
			else
			{
				*fib = *pfib + *ppfib;
			}
			/*---------------*/

			/* Printing current Fibonacci number */
			printf("fib(%d) = %lu\t",i,*fib);
			printf("fib(%d) = %lu\t",i-1,*pfib);
			printf("fib(%d) = %lu\n",i-2,*ppfib);
			/*---------------*/
		}
		else if(pid>0)
		{
			/* parent process is executing */
			printf("Parent process waiting: %d \n",getpid());

			wpid = wait(&status);
			proceed=1;
			printf("Child process ended: %d with status=%d\n\n",wpid,status);
			/*printf("fib = %d\t",*fib);
			printf("pfib = %d\t",*pfib);
			printf("ppfib = %d\n",*ppfib);
			/*---------------*/
		}
		i++;
	}
	/*---------------*/

	printf("\n");
	return 0;
}
