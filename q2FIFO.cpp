#include <bits/stdc++.h>
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

bool visited[501][11]={false};
int a,r;
int numberOfFrames[11];
vector<int> v[11];
int pageFault[11]={0};

void *func(void *id)
{
	if(!visited[a][r])
	{
		v[r].push_back(a);
		visited[a][r]=true;
		int temp;
		if(v[r].size()>numberOfFrames[r])temp=v[r][0],v[r].erase(v[r].begin()),visited[temp][r]=false;
		cout<<"1 Page Fault in "<<r<<endl;
		pageFault[r]++;
	}
	cout<<"State of Memory "<<r+1<<endl;
	for(int j=0;j<v[r].size();j++)cout<<v[r][j]<<" ";
	cout<<endl;
}

void func1(int i)
{
	if(!visited[a][i])
	{
		v[i].push_back(a);
		visited[a][i]=true;
		int temp;
		if(v[i].size()>numberOfFrames[i])temp=v[i][0],v[i].erase(v[i].begin()),visited[temp][i]=false;
		cout<<"1 Page Fault in "<<i+1<<endl;
		pageFault[i]++;
	}
	cout<<"State of Memory "<<i+1<<endl;
	for(int j=0;j<v[i].size();j++)cout<<v[i][j]<<" ";
	cout<<endl;
}


int main(int argc, char const *argv[])
{
	int n=0;
	cout<<"Enter Number Of Page Frames:"<<endl;
	for(int i=0;i<11;i++)
	{
		cin>>numberOfFrames[i];
		if(numberOfFrames[i]==-1)
		{
			break;
		}
		n++;
	}
	pthread_t p[n];
	for(int j=0;j<n;j++)
	{
			// r=j;
			pthread_create(&p[j], NULL, func, (void *)j);	
	}	
	for(int i=0;i<500;i++)
	{
		cout<<"Enter The page Number to be accessed:"<<endl;
		cin>>a;
		if(a==-1)
		{
			cout<<"You Entered -1 , Terminating Program!!"<<endl;
			break;
		}
		for(int j=0;j<n;j++)
		{
			func1(j);
			cout<<"Total page faults in Memory "<<j+1<<" are "<<pageFault[j]<<endl;
		}
		
		// for(int j=0;j<n;j++)
		// {
		// 	pthread_join(p[j], NULL);		
		// }
	}
	return 0;
}
