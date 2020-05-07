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

void PriorityNonPreemtive(ll arrivalTime[],ll CPUburstTime[], ll Priority[], ll n , vpll u)
{
	ll waiting[n+5]={0},turnAround[n+5]={0},avgwaiting=0,avgturnaround=0;
	priority_queue< pll, vpll, greater< pll > > priQue;
	ll cnt=0;
	ll time=0;
	ll index=0;
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
			ll temp=time-arrivalTime[front.y];
			temp=max(0LL,temp);
			waiting[front.y]=temp;
			turnAround[front.y]=temp+CPUburstTime[front.y];
			time+=CPUburstTime[front.y];
			cnt++;
		}
		else
			time++;
	}
	rep(i,n)
	{	
		avgwaiting+=waiting[i];
		avgturnaround+=turnAround[i];
		cout<<"waiting time and response time for process" sp u[i].y sp "is :"<<waiting[i]<<endl;
		cout<<"turn around time for process" sp u[i].y sp "is :"<<turnAround[i]<<endl;
	}
	cout<<"average waiting and response time is :"<<(1.0*avgwaiting)/n<<endl; 
	cout<<"average turnaround time is :"<<(1.0*avgturnaround)/n<<endl;
}

int main(int argc, char const *argv[])
{
	sync;
	freopen("input.dat","r",stdin);
	freopen("output3.dat","w",stdout);
	ll n;
	cin>>n;
	ll arrivalTime[n+5]={0},CPUburstTime[n+5]={0},Priority[n+5]={0};
	vector< pair <ll, pll > >v;
	vpll u;
	rep(i,n)
	{
		cin>>arrivalTime[i]>>CPUburstTime[i]>>Priority[i];
		v.pb({arrivalTime[i],{CPUburstTime[i],Priority[i]}});
		u.pb({arrivalTime[i],i+1});
	}
	ll MLFQ,quantum;
	cin>>quantum;
	cin>>MLFQ;
	sort(v.begin(),v.end());
	sort(u.begin(),u.end());
	rep(i,n)arrivalTime[i]=v[i].x,CPUburstTime[i]=v[i].y.x,Priority[i]=v[i].y.y;
	PriorityNonPreemtive(arrivalTime,CPUburstTime,Priority,n,u);
	return 0;
}
