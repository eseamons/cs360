#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <semaphore.h>
#define NTHTHREADS     10
#define NQUEUE 20

// what is mutual exclusion
sem_t empty, full, mutex;




class myqueue {
	std::queue <int> stlqueue;
	public:
	void push (int sock) {
		sem_wait(&empty);
		sem_wait(&mutex);
		stlqueue.push(sock);
		sem_post(&mutex);
		sem_post(&full);
	}
	int pop () {
		sem_wait(&full);
		sem_wait(&mutex);
		int rval = stlqueue.front();
		stlqueue.pop();
		sem_post(&mutex);
		sem_post(&empty);
		return (rval);
	}

} sockqueue;




void *howdy(void *arg)
{
/* for(;;) {
	sock = dequeue()
	Read request
	write response
	close	
}

*/

	for(;;) {
		std::cout << "GOT " << sockqueue.pop()<<std::endl;
	}
   // long tid;
   // tid = (long)arg;
   // printf("Hello World! It's me, thread #%ld!\n", tid);
}




//   pthread_exit(NULL);
int main (int argc, char *argv[])
{
   long threadid;
   pthread_t threads[NTHTHREADS];
   sem_init(&mutex, PTHREAD_PROCESS_PRIVATE, 1);
   sem_init(&full, PTHREAD_PROCESS_PRIVATE, 0);
   sem_init(&empty, PTHREAD_PROCESS_PRIVATE, NQUEUE);


   for(threadid = 0; threadid < NTHTHREADS; threadid++) {
      pthread_create(&threads[threadid], NULL, howdy, (void *)threadid);
   }

   for(int i = 0; i < NQUEUE; i++) {
   		sockqueue.push(i);
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