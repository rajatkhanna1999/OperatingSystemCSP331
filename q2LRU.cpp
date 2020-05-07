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
vector<int> pages;
unordered_set<int> s[11]; 
unordered_map<int, int> indexes[11]; 

void pageFaults(int a,int i,int id) 
{ 
	int page_faults = 0;
	int capacity=numberOfFrames[id]; 
	if (s[id].size() < capacity) 
	{ 
		if (s[id].find(a)==s[id].end()) 
		{ 
			s[id].insert(a);  
			cout<<"1 Page Fault in memory "<<id+1<<endl; 
			pageFault[id]++;
		} 
		indexes[id][a] = i; 
	} 
	else
	{ 
		if (s[id].find(a) == s[id].end()) 
		{ 
			int lru = INT_MAX, val; 
			for (auto it=s[id].begin(); it!=s[id].end(); it++) 
			{ 
				if (indexes[id][*it] < lru) 
				{ 
					lru = indexes[id][*it]; 
					val = *it; 
				} 
			} 
			s[id].erase(val); 
			s[id].insert(a); 
			cout<<"1 Page Fault in memory "<<id+1<<endl;
			pageFault[id]++;
		} 
		indexes[id][a] = i; 
	} 
	for (auto it=s[id].begin(); it!=s[id].end(); it++)cout<<*it<<" ";
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
	for(int i=0;i<100;i++)
	{
		cout<<"Enter The page Number to be accessed:"<<endl;
		int a;
		cin>>a;
		pages.push_back(a);
		if(a==-1)
		{
			cout<<"You Entered -1 , Terminating Program!!"<<endl;
			break;
		}
		for(int j=0;j<n;j++)
		{
			pageFaults(a,i+1,j);
			cout<<"Total page faults in Memory "<<j+1<<" are "<<pageFault[j]<<endl;
		}
	}
	return 0;
}
