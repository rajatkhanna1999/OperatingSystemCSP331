#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

sem_t resource;
sem_t rmutex;
int numWriters;
int numReaders;
int readcount = 0;
int readerWait[1005]={0};
int writerWait[1005]={0};

// Reader
void *ReaderFunc(void *readerId) {
	//for(int i=0;i<5;i++){
	int id = (int) readerId;
	int j=0; 		
	sem_wait(&rmutex);
	readcount ++;
	if (readcount == 1) 
	{
		sem_wait(&resource);
		printf("First reader\n");
	}
	sem_post(&rmutex);

	// Reading and Sleeping

	int sleepTime = 1 + (rand() % 2);
	printf("Reader %d is reading for %d seconds.\n", id + 1, sleepTime);
	sleep(sleepTime);

	sem_wait(&rmutex);
	readcount --;
	if (readcount == 0) 
	{
		for(j=0;j<numWriters;j++)
		{
			if(writerWait[j]==1)printf("Writer %d is waiting.\n",(j+1));
		}
		printf("Last reader\n");
		sem_post(&resource);
	}
	sem_post(&rmutex);
	//}

}

// writer
void *WriterFunc(void *writerId) {
	//for(int i=0;i<5;i++){
	int id = (int) writerId;
	int j=0;
	writerWait[id]=1;
	sem_wait(&resource);
	writerWait[id]=0;

	// Waiting and Sleeping
	int sleepTime = 1 + (rand() % 2);
	printf("Writer %d writing for %d seconds.\n", id + 1, sleepTime);
	sleep(sleepTime);

	for(j=0;j<numWriters;j++){
		if(writerWait[j] == 1){
			printf("Writer %d is waiting.\n",(j+1));
		}
	}

	sem_post(&resource);
	//}
}

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Usage of the program: ./reader_writer <no. of writers> <no. of readers>\n");
		return 0;
	}

	 numWriters = atoi(argv[1]);
	printf("Number of writers are : %d\n", numWriters);

	 numReaders = atoi(argv[2]);
	printf("Number of readers are: %d\n", numReaders);

	// Initialize the semaphore variables to make them work as Locks
	sem_init(&resource, 0, 1);
	sem_init(&rmutex, 0, 1);

	// Threads for readers and writers
	pthread_t reader[numReaders], writer[numWriters];



	//Threads for writers
	for (int i = 0; i < numWriters; i ++) {
		pthread_create(&writer[i], NULL, WriterFunc, (void *)i);
	}

	for (int i = 0; i < numReaders; i ++) {
		pthread_create(&reader[i], NULL, ReaderFunc, (void *)i);
	}

	// Threads for readers

	// int wCount=0,rCount=0;
	// for(int i=0;i<numWriters+numReaders;i++)
	// {
	// 	int num = 1 + rand()%2;
	// 	if(num==1 && wCount<=numWriters)pthread_create(&writer[i], NULL, WriterFunc, (void *)wCount),wCount++;
	// 	else pthread_create(&reader[i], NULL, ReaderFunc, (void *)rCount),rCount++;
	// }
	
	// Waiting for writer threads
	for (int i = 0; i < numWriters; i ++) {
		pthread_join(writer[i], NULL);	
	}

	// Waiting for reader threads
	for (int i = 0; i < numReaders; i ++) {
		pthread_join(reader[i], NULL);
	}

	return 0;
}
