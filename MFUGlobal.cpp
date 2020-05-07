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

bool is_Present[501][11]={false};
int page_refrence[502]={0};
int num_page_refrence = 0;
int num_memory = 0;
int ref_page,r;
int numberOfFrames[11];
vector<int> memory[11];
int pageFault[11]={0};


vector<int> pages;
unordered_set<int> s[11]; 
unordered_map<int, int> frequency[11]; 

void MFU(int id, int i) 
{  
	if (s[id].size() < numberOfFrames[id]) 
	{ 
		if (s[id].find(ref_page)==s[id].end()) 
		{ 
			s[id].insert(ref_page);  
			pageFault[id]++;
		} 
		frequency[id][ref_page]++; 
	} 
	else
	{ 
		if (s[id].find(ref_page) == s[id].end()) 
		{ 
			int mfu = 0, val; 
			for (auto it=s[id].begin(); it!=s[id].end(); it++) 
			{ 
				if (frequency[id][*it] > mfu) 
				{ 
					mfu = frequency[id][*it]; 
					val = *it; 
				} 
			} 
			s[id].erase(val); 
			s[id].insert(ref_page); 
			pageFault[id]++;
		}
		else{
			cout<<"Page"<<ref_page<<" is already present in memory "<<(id+1)<<endl;
		} 
		frequency[id][ref_page]++; 
	} 

} 

void snapshot()
{
	for(int q=0;q<num_memory;q++){
		cout<<"State of Memory "<<q+1<<endl;
		for(auto it=s[q].begin(); it!=s[q].end(); it++){
			cout<<*it<<" ";
		}
		cout<<endl;
		cout<<"Total number of page faults till now in memory "<<(q+1)<<" are: "<<pageFault[q]<<endl;	
		cout<<endl;
	}
}

int main(int argc, char const *argv[])
{
	int n;
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	for(int i=0;i<500;i++){
		int p;
		cin>>p;
		if(p!=-1){
			page_refrence[i] = p;
			num_page_refrence++;
		}else{
			break;
		}
	}
	for(int i=0;i<11;i++)
	{
		cin>>numberOfFrames[i];
		if(numberOfFrames[i]==-1)
		{
			break;
		}
		num_memory++;
	}

	cin>>n;
	for(int i=0;i<num_page_refrence;i++)
	{
		ref_page = page_refrence[i];
		if(i==n) snapshot();
		cout<<"page Number to be accessed is : "<<page_refrence[i]<<endl;
		for(int j=0;j<num_memory;j++)
			MFU(j,i+1);
	}
	return 0;
}
