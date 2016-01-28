#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NTHTHREADS     20

void *howdy(void *arg)
{

   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
}
//   pthread_exit(NULL);
int main (int argc, char *argv[])
{
   int threadid;
   pthread_t threads[NTHTHREADS];
   for(threadid = 0; threadid < NTHREADS; threadid++) {
      pthread_create(&thread[threadid], NULL, howdy, (void *)threadid);
   }

   pthread_exit(NULL);
   
   return 0;
}