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
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <linux/unistd.h>
#include <sys/time.h>
#include <unistd.h>
#include <wait.h>
#include <sys/syscall.h>
#include <errno.h>
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
typedef int                     ll;
typedef double                  ld;
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

pid_t getpid( )
{
    return syscall( __NR_getpid );
}

int main()
{
    /*#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif*/
    cpu_set_t set;
    struct sched_param prio_param;
    int prio_max;

    CPU_ZERO( &set );
    CPU_SET( 0, &set );
    memset(&prio_param,0,sizeof(struct sched_param));

    if (sched_setaffinity(getpid(),sizeof(cpu_set_t),&set))
    {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }

    if((prio_max = sched_get_priority_max(SCHED_FIFO)) < 0 )perror("sched_get_priority_max");

    prio_param.sched_priority = prio_max;
    if(sched_setscheduler(getpid(),SCHED_FIFO,&prio_param) < 0 )
    {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }
    ll i=0;
    ll ret=-1;
    ll firstpipe[2];
    ll secondpipe[2];
    ll timepipe[2];
    ll kyu;
    char string[] = "Hello, world!\n";
    char temp[] = "Sumit Gemini!\n";
    char readbuffer[80];
    char tempbuffer[80];
    struct timeval start,end;
    if (pipe(firstpipe) == -1) 
    {
        fprintf(stderr, "Error!!! (Parent)Failed to create pipe\n");
        return -1;
    }
    if (pipe(secondpipe) == -1) 
    {
        fprintf(stderr, "Error!!! (Parent)Failed to create second pipe\n");
        return -1;
    }
    if (pipe(timepipe) == -1) 
    {
        fprintf(stderr, "Error!!! (Parent)Failed to create time pipe\n");
        return -1;
    }
    if((ret=fork())==-1)
        perror("fork");
    else if(ret==0)
    {
        ll n=-1;
        printf("Child  = %d\n",getpid());

        for(n=0;n<10;n++)
        {
            kyu = read(firstpipe[0], readbuffer, sizeof(readbuffer));
            write(secondpipe[1], temp, strlen(temp)+1);
        }

        gettimeofday(&end,0);
        n = sizeof(struct timeval);

        if( write(timepipe[1],&end,sizeof(struct timeval)) != n )
        {
            fprintf(stderr, "Error!!! (Child)Failed to write in time pipe\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        ld switch_time;
        ll n=-1;
        printf("Parent  = %d\n",getpid());
        gettimeofday(&start,0);

        for(n=0;n<10;n++)
        {
            write(firstpipe[1], string, strlen(string)+1);
            read(secondpipe[0], tempbuffer, sizeof(tempbuffer));
        }

        n = sizeof(struct timeval);
        if( read(timepipe[0],&end,sizeof(struct timeval)) != n )
        {
            fprintf(stderr, "Error!!! (Parent)Failed to read from time pipe\n");
            exit(EXIT_FAILURE); 
        }

        wait(NULL);
        switch_time = ((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))/1000.0;
        printf("context switch between two processes: %0.6lfms\n",switch_time/(5*2));
    }   
    return 0;
}
