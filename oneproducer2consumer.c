// One Producer and 2 Consumers
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define Maxxx 10
int BUFFER_SIZE; 
int *buffer;     
int filled = 0;  
int used = 0;    
int val = 0;     

sem_t empty;    
sem_t full;     
sem_t mutex; 
sem_t consumermutex;   

void produce(int value) {
	buffer[filled] = value;
	printf("The value PRODUCED is: %d\n", value);
	filled = (filled + 1) % BUFFER_SIZE;
}

void *producer(void *producer_id) {
	for(int i=0;i<3*Maxxx;i++)
	{
		int id = (int) producer_id;	

		sem_wait(&empty); 
		sem_wait(&mutex); 
		val = (val + 1);
		produce(val); 

		int sleep_time = 1 + (rand() % 3);
		printf("Producer 1 waiting for %d seconds.\n", id, sleep_time);
		sleep(sleep_time);

		sem_post(&mutex);	
		sem_post(&full);
	}
}

void *consumer1(void *consumer_id) {
	for(int i=0;i<Maxxx;i++)
	{
		int id = (int) consumer_id;
		sem_wait(&full);
		sem_wait(&consumermutex); 
		sem_wait(&mutex);

		int tmp = buffer[used];
		used = (used + 1) % BUFFER_SIZE;
		printf("The value consumed by consumer-1 is: %d\n", tmp); 

		int sleep_time = 1 + (rand() % 3);
		printf("Consumer %d waiting for %d seconds.\n", id, sleep_time);
		sleep(sleep_time);

		sem_post(&consumermutex);
		sem_post(&mutex); 
		sem_post(&empty);	
	}
}

void *consumer2(void *consumer_id) {
	for(int i=0;i<Maxxx;i++)
	{
		int id = (int) consumer_id;
		sem_wait(&full);
		sem_wait(&consumermutex); 
		sem_wait(&mutex);
		
		int sleep_time,tmp;
		tmp = buffer[used];
		used = (used + 1) % BUFFER_SIZE;
		printf("The 1st value consumed by consumer-2 is: %d\n", tmp); 
		sleep_time = 1 + (rand() % 3);
		printf("Consumer %d waiting for %d seconds.\n", id, sleep_time);
		sleep(sleep_time);

		sem_post(&mutex);
		sem_post(&empty);

		sem_wait(&full);
		sem_wait(&mutex);

		tmp = buffer[used];
		used = (used + 1) % BUFFER_SIZE;
		printf("The 2nd value consumed by consumer-2 is: %d\n", tmp); 
		sleep_time = 1 + (rand() % 3);
		printf("Consumer %d waiting for %d seconds.\n", id, sleep_time);
		sleep(sleep_time);

		sem_post(&consumermutex);
		sem_post(&mutex); 
		sem_post(&empty);
	}
}

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("Usage of the program: ./producer_consumer <buffer size>\n");
		return 0;
	}

	int num_producers=1;
	int num_consumers=2;

	BUFFER_SIZE = atoi(argv[1]);
	printf("The size of the buffer is: %d\n", BUFFER_SIZE);

	buffer = (int *) malloc (BUFFER_SIZE * sizeof(int));

	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);
	sem_init(&consumermutex, 0, 1);

	pthread_t p, c1 , c2;

	int i=1;
	pthread_create(&p, NULL, producer, (void *)i);
	i=1;
	pthread_create(&c1, NULL, consumer1, (void *)i);
	i=2;
	pthread_create(&c2, NULL, consumer2, (void *)i);

	pthread_join(p, NULL);	
	pthread_join(c1, NULL);
	pthread_join(c2, NULL);

	return 0;
}