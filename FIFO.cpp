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

void FIFO(ll arrivalTime[],ll CPUburstTime[], ll n , vpll u)
{
	ll turnaroundTime=0,waitingTime=0,responseTime=0;
	ll currTime=0;
	rep(i,n)
	{
		if(i==0)
			currTime=arrivalTime[i]+CPUburstTime[i];
		else 
		{
			if(arrivalTime[i]<=currTime)
				currTime+=CPUburstTime[i];
			else
				currTime=arrivalTime[i]+CPUburstTime[i];
		}
		cout<<"turn Around Time for process" sp u[i].y sp "is:" sp currTime-arrivalTime[i]<<endl;
		cout<<"waiting and response Time" sp u[i].y sp " is:" sp currTime-arrivalTime[i]-CPUburstTime[i]<<endl;
		responseTime+=(currTime-arrivalTime[i]-CPUburstTime[i]);
		turnaroundTime+=(currTime-arrivalTime[i]);
	}
	cout<<"Average Turn Around Time is:";
	cout<<(1.0*turnaroundTime)/n<<endl;
	cout<<"Average response Time is:";
	cout<<(1.0*responseTime)/n<<endl;
}

int main(int argc, char const *argv[])
{
	sync;
	freopen("input.dat","r",stdin);
	freopen("output.dat","w",stdout);
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
	FIFO(arrivalTime,CPUburstTime,n,u);
	return 0;
}
