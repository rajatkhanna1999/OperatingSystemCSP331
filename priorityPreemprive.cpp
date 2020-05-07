#include <bits/stdc++.h>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <utility>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <iomanip>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp> 
//setbase - cout << setbase (16); cout << 100 << endl; Prints 64
//setfill -   cout << setfill ('x') << setw (5); cout << 77 << endl; prints xxx77
//setprecision - cout << setprecision (14) << f << endl; Prints x.xxxx
//cout.precision(x)  cout<<fixed<<val;  // prints x digits after decimal in val
using namespace std;
using namespace __gnu_pbds;

#define sp << " " <<
#define mod 1000000007
#define PI 3.14159265358979323
#define y second
#define x first
#define pb push_back
#define mp make_pair
#define sz(a) a.size()
#define pqueue priority_queue< ll >
//#define pdqueue priority_queue< ll,vector<ll> ,greater< ll > >
#define codejamDebug(x) cout<<"Case #"<<x<<": ";
#define debug(x) cout<<#x<<" :"<<x<<endl;
#define rep(i,n) for(long long i=0;i<n;i++)
#define f(i,a,b) for(long long i = a; i < b; i++)
#define fd(i,a,b) for(long long i = a; i >=b; i--)
#define sync ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long int           ll;
typedef long double             ld;
typedef unsigned long long int ull;
typedef vector <int>            vi;
typedef vector <ll>            vll;
typedef pair <int, int>        pii;
typedef pair <ll, ll>          pll;
typedef vector < pii >        vpii;
typedef vector < pll >        vpll;
typedef vector <string>         vs;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//Handle:cyber_rajat

struct Compute
{
	ll arrivalTime;
	ll burstTime,completionTime;
	ll turnAroundTime,waitingTime;
	ll priority,burstCopy,id;
};

bool compare(Compute a , Compute b)
{
	return a.priority<b.priority;
}

bool compare1(Compute a , Compute b)
{
	return a.arrivalTime<b.arrivalTime;
}

void PriorityPreemptive(Compute process[], ll n)
{
	sort(process,process+n,compare1);
	ll i=0,count=0,j=0;
	while(count<n)
	{
		for(j=0;j<n;j++)
		{
			if(process[j].arrivalTime>i)
				break;
			
		}
		sort(process,process+j,compare);
		if(j>0)
		{
			for(j=0;j<n;j++)
			{
				if(process[j].burstTime!=0){
					break;
				}
			}
			if(process[j].arrivalTime > i)i=process[j].arrivalTime;
			process[j].completionTime=i+1;
			process[j].burstTime--;
		}
		i++;
		count=0;
		for(j=0;j<n;j++)if(process[j].burstTime==0)count++;
	}
	rep(i,n)
	{
		process[i].turnAroundTime=process[i].completionTime-process[i].arrivalTime;
		process[i].waitingTime=process[i].turnAroundTime-process[i].burstCopy;
		cout<<process[i].id;
		cout<<" ";
		cout<<process[i].turnAroundTime sp process[i].waitingTime<<endl;
	}
}

void PriorityPreemptive1(ll arrivalTime[],ll CPUburstTime[], ll Priority[], ll n , vpll u)
{
	ll BurstTimeCopy[n+5]={0};
	rep(i,n)BurstTimeCopy[i]=CPUburstTime[i];
	ll waiting[n+5]={0},turnAround[n+5]={0},response[n+5]={0},avgresponse=0,avgwaiting=0,avgturnaround=0;
	priority_queue< pll, vpll, greater< pll > > priQue;
	ll cnt=0;
	ll time=0;
	ll index=0;
	ll firstArrival[n+5],lastArrival[n+5];
	memset(firstArrival,-1,sizeof(firstArrival));
	memset(lastArrival,-1,sizeof(lastArrival));
	while(cnt!=n)
	{
		while(index<n && arrivalTime[index]<=time)
		{
			priQue.push({Priority[index],index});
			index++;
		}
		if(!priQue.empty())
		{
			pll front=priQue.top();
			priQue.pop();
			if(firstArrival[front.y]==-1)
				firstArrival[front.y]=time;
			BurstTimeCopy[front.y]--;
			time++;
			if(BurstTimeCopy[front.y]>0)
				priQue.push({front.x,front.y});
			else
			{
				cnt++;
				lastArrival[front.y]=time;
			}
		}
		else
			time++;
	}
	rep(i,n)
	{	
		turnAround[i]=lastArrival[i]-arrivalTime[i];
		waiting[i]=turnAround[i]-CPUburstTime[i];
		response[i]=firstArrival[i]-arrivalTime[i];
		avgwaiting+=waiting[i];
		avgturnaround+=turnAround[i];
		avgresponse+=response[i];
		cout<<"waiting time for process" sp u[i].y sp "is :"<<waiting[i]<<endl;
		cout<<"response time for process" sp u[i].y sp "is :"<<response[i]<<endl;
		cout<<"turn around time for process" sp u[i].y sp "is :"<<turnAround[i]<<endl;
	}
	cout<<"average waiting time is :"<<(1.0*avgwaiting)/n<<endl; 
	cout<<"average response time is :"<<(1.0*avgresponse)/n<<endl; 
	cout<<"average turnaround time is :"<<(1.0*avgturnaround)/n<<endl;
}

int main(int argc, char const *argv[])
{
	sync;
	freopen("input.dat","r",stdin);
	freopen("output4.dat","w",stdout);
	ll n;
	cin>>n;
	Compute process[n+5];
	ll arrivaltime[n+5]={0},CPUburstTime[n+5]={0},Priority[n+5]={0};
	vector< pair <ll, pll > >v;
	vpll u;
	rep(i,n)
	{
		process[i].id=i+1;
		cin>>process[i].arrivalTime;
		cin>>process[i].burstTime;
		cin>>process[i].priority;
		process[i].burstCopy=process[i].burstTime;
		arrivaltime[i]=process[i].arrivalTime;
		CPUburstTime[i]=process[i].burstTime;
		Priority[i]=process[i].priority;
		v.pb({arrivaltime[i],{CPUburstTime[i],Priority[i]}});
		u.pb({arrivaltime[i],i+1});
	}
	ll MLFQ,quantum;
	cin>>quantum;
	cin>>MLFQ;
	sort(v.begin(),v.end());
	sort(u.begin(),u.end());
	rep(i,n)arrivaltime[i]=v[i].x,CPUburstTime[i]=v[i].y.x,Priority[i]=v[i].y.y;
	//PriorityPreemptive(process,n);
	PriorityPreemptive1(arrivaltime,CPUburstTime,Priority,n,u);
	
	return 0;
}
