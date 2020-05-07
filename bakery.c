#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define	NTURN 50

#define	RAND_MAX 0xFFFFFFFFu // Maximum 32-bit unsigned number

int visited[100];
int tickets[100];
int value;

void *thread1(void *args) {
	int x = *(int *)args;
	int maxi=0;
	for(int j=0;j<value;j++)
	{
		if(tickets[j]>maxi)maxi=tickets[j];
	}
	for(int i=0; i < NTURN; i++)
	{
		visited[x]=1;
		tickets[x]=maxi+1;
		visited[x]=0;
		for(int j=0;j<value;j++)
		{
			if(j==x)continue;

			while(visited[j]!=0);

			while(tickets[j]!=0 && tickets[j]<tickets[x]);

			if(tickets[j]==tickets[x] && j<x)
			{
				while(tickets[j]!=0);
			}
		}
	}
	printf("%d using resource\n", x);
	tickets[x]=0;
}

void main(int argc, char** argv) {
	srand(time(NULL));
	if (argc != 2) {
    fprintf(stderr, "usage: main-first <loopcount>\n");
    exit(1);
    }
    value = atoi(argv[1]);
   // memset((void*)tickets, 0, sizeof(tickets)); 
   // memset((void*)visited, 0, sizeof(visited)); 
    pthread_t threads[value];     
	//Creating array of Threads

    for (int i = 0; i < value; ++i)
        pthread_create(&threads[i], NULL, thread1, &i); 
  
    for (int i = 0; i < value; ++i)
        pthread_join(threads[i], NULL); 
 
	return;
}
