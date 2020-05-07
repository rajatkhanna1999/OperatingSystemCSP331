#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
sem_t *forks;
sem_t mutex;
pthread_t philosophers[7];

void think(int i)
{
	int s = rand()%3;
	printf("Philosopher %d will think for %d seconds\n", i, s);
	sleep(s);
}

void pick(int i)
{
	int	right = (i + 1) % 5;
	int left = (i) % 5;
	printf("Philosopher %d is waiting to pick up fork %d\n", i, right);
	sem_wait(&forks[right]);
	printf("Philosopher %d picked up fork %d\n", i, right);
	printf("Philosopher %d is waiting to pick up fork %d\n", i, left);
	sem_wait(&forks[left]);
	sem_wait(&mutex);
	printf("Philosopher %d picked up fork %d\n", i, left);
}

void eat(int i) {
	int eatTime = rand()%3;
	printf("Philosopher %d will eat for %d seconds\n", i, eatTime);
	sleep(eatTime);
	sem_post(&mutex);
}

void drop(int i) {
	printf("Philosopher %d will drop his forks\n", i);
	sem_post(&forks[(i + 1) % 5]);
	sem_post(&forks[(i) % 5]);
}

void *philosopher(void * i)
{
	while(1)
	{
		think(i);
		pick(i);
		eat(i);
		drop(i);
	}
}

int main()
{
	int i;
	forks = malloc(sizeof(sem_t)*5);
	sem_init(&mutex, 0, 1);
	int j = rand()%5;
	for (i = 0; i < 5; ++i) 
	{
		sem_init(&forks[i],0,1);
	}
	for (i = 0; i < 5; ++i) 
	{
		if(i!=j)
			pthread_create(&philosophers[i], NULL, philosopher, (void *)(i));
	}
	for (i = 0; i < 5; ++i) 
	{
		pthread_join(philosophers[i], NULL);
	}
	return 0;
}
