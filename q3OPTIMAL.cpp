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

bool is_Present[502][12]={false};
int page_refrence[502]={0};
int num_page_refrence = 0;
int num_memory = 0;
int ref_page,r;
int numberOfFrames[12];
vector<int> memory[12];
int pageFault[12]={0};


int findOptimal(int id,int i){
	int res = -1, farthest = id; 
    for (int j = 0; j < memory[i].size(); j++) { 
        int k; 
        for (k = id; k < num_page_refrence; k++) { 
            if (memory[i][j] == page_refrence[k]) { 
                if (k > farthest) { 
                    farthest = k; 
                    res = j; 
                } 
                break; 
            } 
        } 
        if (k == num_page_refrence) 
            return j; 
    } 
    if(res==-1)return 0;
    return res;
}

void func(int id,int i)
{
	if(!is_Present[ref_page][i])
	{
		if(memory[i].size() < numberOfFrames[i]){
			memory[i].push_back(ref_page);
			is_Present[ref_page][i]=true;
			pageFault[i]++;
		}
		else{
			int temp;
			temp=findOptimal(id,i);
			is_Present[memory[i][temp]][i]=false;
			memory[i].erase(memory[i].begin() + temp);
			memory[i].push_back(ref_page);
			is_Present[ref_page][i]=true;
			pageFault[i]++;
		}
		
	}
	else{
		cout<<"Page"<<ref_page<<" is already present in memory "<<(i+1)<<endl;
 	}

}

void snapshot(){
	for(int q=0;q<num_memory;q++){
		cout<<"State of Memory "<<q+1<<endl;
		for(int k=0;k<memory[q].size();k++){
			cout<<memory[q][k]<<" ";
		}
		cout<<endl;
		cout<<"Total number of page faults till now in memory "<<(q+1)<<" are: "<<pageFault[q]<<endl;	
		cout<<endl;
	}
}



int main(int argc, char const *argv[])
{
	
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);	
	int n;
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
	for(int i=0;i<10;i++)
	{
		cin>>numberOfFrames[i];
		if(numberOfFrames[i]==-1)
		{
			break;
		}
		num_memory++;
	}
	
	cin>>n;
	pthread_t p[n];
	for(int i=0;i<num_page_refrence;i++)
	{
		ref_page = page_refrence[i];
		snapshot();
		cout<<"page Number to be accessed is : "<<page_refrence[i]<<endl;
		for(int j=0;j<num_memory;j++)func(i,j);
	}
	return 0;
}