#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

#define Maxx 100
pthread_t phil[Maxx];
sem_t forks[Maxx];
int N;

int right(int i)
{
    return i;
}

int left(int i)
{
    if(i==0)return N-1;
    return i-1;
}

void getForks(int i)
{
    if (i % 2 == 0)
    {
        sem_wait(&forks[left(i)]);
        sem_wait(&forks[right(i)]);
    }
    else{
        sem_wait(&forks[right(i)]);
        sem_wait(&forks[left(i)]);
    }
}

void putForks(int i)
{
    sem_post(&forks[right(i)]);
    sem_post(&forks[left(i)]);
}

void philFunc(int id)
{
    getForks(id);
    printf("Philosopher [%d] eating\n", id);
    putForks(id);
}
// odd ones pick right first, even ones left
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage of the program: ./dining_philosopher <no. of philosophers>\n");
        return 0;
    }

    N = atoi(argv[1]);
    printf("Number of philosphers: %d\n",N);
    for (int i = 0; i < N; i++)
    {
        sem_init(&forks[i], 0, 1);
    }

    int success;
    for (int i = 0; i < N; i++)
    {
        pthread_create(&phil[i], NULL, philFunc, i);
    }
    
    for (int i = 0; i < N; i++)
    {
        pthread_join(phil[i], NULL);
    }
}