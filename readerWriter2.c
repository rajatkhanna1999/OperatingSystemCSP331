#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

sem_t resource , rmutex , wmutex , readTry;
int readCount = 0, writeCount = 0;
int numWriters;
int numReaders;
int readcount = 0;
int readerWait[1005]={0};
int writerWait[1005]={0};
// Reader
void *ReaderFunc(void *ReaderId) {
	// for(int i=0;i<5;i++){
	int id = (int) ReaderId;
	int j=0;
	readerWait[id]=1;		
	sem_wait(&readTry);
	readerWait[id]=0;		
	sem_wait(&rmutex); 
	readCount ++;
	if (readCount == 1) 
	{
		sem_wait(&resource);
		printf("First reader\n");
	}
	sem_post(&rmutex);
	sem_post(&readTry);

	// Reading process sleep for some time
	int sleepTime = 1 + (rand() % 2);
	printf("Reader %d is reading for %d seconds.\n", id + 1, sleepTime);
	sleep(sleepTime);

	for(j=0;j<numWriters;j++)
	{
        if(writerWait[j] == 1)printf("Writer %d is waiting.\n", j + 1);	
	}

	sem_wait(&rmutex);
	readCount --;
	if (readCount == 0) 
	{
		printf("Last reader\n");
		sem_post(&resource);
	}

	sem_post(&rmutex);
	//}
}

// Writer
void *WriterFunc(void *writerId) {
	//for(int i=0;i<5;i++){
	int id = (int) writerId;
	int j=0;
	writerWait[id]=1;
	sem_wait(&wmutex);
	writerWait[id]=0;
	writeCount ++;
	if (writeCount == 1)
	{
		sem_wait(&readTry);
		printf("First writer\n");
	}
	sem_post(&wmutex);

	sem_wait(&resource); 
	
	// Writing sleep for random time
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

	sem_post(&resource);

	sem_wait(&wmutex);
	writeCount --;
	if (writeCount == 0) 
	{
		printf("Last writer\n");
		sem_post(&readTry);
	}
	sem_post(&wmutex);
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

	// initializing the semaphore variables
	sem_init(&resource, 0, 1),sem_init(&wmutex, 0, 1);
	sem_init(&readTry, 0, 1),sem_init(&rmutex, 0, 1);

	pthread_t reader[numReaders], writer[numWriters];
	
	// Threads for writers
	for (int i = 0; i < numWriters; i ++) {
		pthread_create(&writer[i], NULL, WriterFunc, (void *)i);
	}

	// Threads for readers
	for (int i = 0; i < numReaders; i ++) {
		pthread_create(&reader[i], NULL, ReaderFunc, (void *)i);
	}
	
	// Wait for Writer threads
	for (int i = 0; i < numWriters; i ++) {
		pthread_join(writer[i], NULL);	
	}

	// Wait for Reader threads
	for (int i = 0; i < numReaders; i ++) {
		pthread_join(reader[i], NULL);
	}

	return 0;
}
