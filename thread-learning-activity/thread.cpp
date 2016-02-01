#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NTHTHREADS     20

void *howdy(void *arg)
{
/* for(;;) {
	sock = dequeue()
	Read request
	write response
	close	
}

*/
   long tid;
   tid = (long)arg;
   printf("Hello World! It's me, thread #%ld!\n", tid);
}
//   pthread_exit(NULL);
int main (int argc, char *argv[])
{
   int threadid;
   pthread_t threads[NTHTHREADS];
   for(threadid = 0; threadid < NTHTHREADS; threadid++) {
      pthread_create(&threads[threadid], NULL, howdy, (void *)threadid);
   }


/* 
   set up socket, bind , listen
   for(;;) {
	fd = accept
	enqueue(fd)
   }



*/
   pthread_exit(NULL);
   
   return 0;
}