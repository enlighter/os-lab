#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>		/* including necessary headers */

int main()
{
	pid_t pid, wpid;					//pids of children processes
	int n=0, status=0, i=0;				//integer variables
	static int fib, pfib, ppfib;		//fibonacci variables: <current>, <pre>, <pre-pre>

	/* Declaration and user variable input */
	printf("Fibchild - fibonacci series \n");
	printf("Give the length of fibonacci series to be printed: ");
	scanf("%d",&n);
	printf("\n");
	/*---------------*/

	for(i=1; i<=n; i++)
	{
		pid = fork();
		printf("Child process created: %d \n",pid);
		if(!pid)
		{
			//child process is executing
			printf("Child process executing: %d \n",getpid());

			ppfib = pfib;
			pfib = fib;
			if (i<3)
			{
				fib=1;
			}
			else
			{
				fib = pfib + ppfib;
			}
			/*---------------*/

			printf("fib = %d\t",fib);
			printf("pfib = %d\t",pfib);
			printf("ppfib = %d\n",ppfib);
			/*---------------*/
		}
		else if(pid>0)
		{
			//parent process is executing
			printf("Parent process executing: %d \n",getpid());

			wpid = wait(&status);
			printf("Child process ended: %d \n",wpid);
			/*---------------*/
		}
	}
	/*---------------*/

	printf("\n");
	return 0;
}
