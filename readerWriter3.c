// multiple readers and multiple writers problem using semaphores with threads
// fair solution

#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

sem_t resourceAccess;
sem_t readCountAccess;
sem_t serviceQueue;
int readCount = 0;
int numWriters;
int numReaders;
int readcount = 0;
int readerWait[1005]={0};
int writerWait[1005]={0};

// reader
void *ReaderFunc(void *reader_id) {
	//for(int i=0;i<5;i++){
	int id = (int) reader_id;
	int j=0;		
	sem_wait(&serviceQueue);
	readerWait[id]=1;
	sem_wait(&readCountAccess);
	if (readCount == 0) 
	{
		sem_wait(&resourceAccess);
		printf("First reader\n");
	}
	readCount++;

	sem_post(&serviceQueue);
	sem_post(&readCountAccess);

	// reading process
	// sleep for random time
	readerWait[id]=0;
	int sleepTime = 1 + (rand() % 2);
	printf("Reader %d is reading for %d seconds.\n", id + 1, sleepTime);
	sleep(sleepTime);

	for(j = 0 ; j< numWriters; j++)
	{
		if(writerWait[j]==1)printf("writer %d is waiting .\n", j + 1);	
    }

	sem_wait(&readCountAccess);
	readCount --;
	if (readCount == 0) 
	{
		printf("Last reader\n");
		sem_post(&resourceAccess);
	}

	sem_post(&readCountAccess); 
	//}
}

// writer
void *WriterFunc(void *writerId) {
	//for(int i=0;i<5;i++){
	int id = (int) writerId;
	int j=0;
	sem_wait(&serviceQueue);
	writerWait[id]=1;
	sem_wait(&resourceAccess);

	sem_post(&serviceQueue);
	writerWait[id]=0;
	// writing sleep for some time
	int sleepTime = 1 + (rand() % 2);
	printf("Writer %d writing for %d seconds.\n", id + 1, sleepTime);
	sleep(sleepTime);

	for(j = 0 ; j< numReaders; j++)
	{
		if(readerWait[j]==1)printf("reader %d is waiting .\n", j + 1);	
	}
	for(j = 0 ; j< numWriters; j++)
	{
		if(writerWait[j]==1)printf("writer %d is waiting .\n", j + 1);	
	}

	sem_post(&resourceAccess);
	//}
}

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Usage of the program: ./reader_writer <no. of writers> <no. of readers>\n");
		return 0;
	}

	numWriters = atoi(argv[1]);
	printf("Number of writers: %d\n", numWriters);

	numReaders = atoi(argv[2]);
	printf("Number of readers is: %d\n", numReaders);

	// initialize the semaphore variables
	sem_init(&resourceAccess, 0, 1);
	sem_init(&readCountAccess, 0, 1);
	sem_init(&serviceQueue, 0, 1);
	
	// threads for readers and writers
	pthread_t reader[numReaders], writer[numWriters];
	
	// create threads for writers
	for (int i = 0; i < numWriters; i ++) {
		pthread_create(&writer[i], NULL, WriterFunc, (void *)i);
	}
	
	// create threads for readers
	for (int i = 0; i < numReaders; i ++) {
		pthread_create(&reader[i], NULL, ReaderFunc, (void *)i);
	}
	
	// Wait for writer threads
	for (int i = 0; i < numWriters; i ++) {
		pthread_join(writer[i], NULL);	
	}

	// Wait for Reader threads
	for (int i = 0; i < numReaders; i ++) {
		pthread_join(reader[i], NULL);
	}

	return 0;
}
