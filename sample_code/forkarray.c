#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main ()
{
   int A[10], *B, i, t, pid;

   srand((unsigned int)time(NULL));
   B = (int *)malloc(10 * sizeof(int));
   t = 1 + rand() % 5;
   if ((pid = fork())) {
      for (i=0; i<10; ++i) A[i] = B[9-i] = 10+i;
      printf("Parent process going to sleep for %d seconds\n", t);
      sleep(t);
      printf("Parent process: A = %p, B = %p\n", A, B);
   } else {
      for (i=0; i<10; ++i) A[i] = B[9-i] = i;
      i = t; while (i == t) t = 1 + rand() % 5;
      printf("Child process going to sleep for %d seconds\n", t);
      sleep(t);
      printf("Child process: A = %p, B = %p\n", A, B);
   }
   printf("A[] ="); for (i=0; i<10; ++i) printf(" %d", A[i]); printf("\n");
   printf("B[] ="); for (i=0; i<10; ++i) printf(" %d", B[i]); printf("\n");
   free(B);
   if (pid) wait(NULL);
   exit(0);
}
