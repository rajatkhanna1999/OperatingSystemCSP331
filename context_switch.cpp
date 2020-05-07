#include<bits/stdc++.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <linux/unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#define er stderr, "pipe error\n"
#define e_f EXIT_FAILURE
using namespace std;

int main(){
    cpu_set_t set;
    struct sched_param prio_param;
    int max_priority;

    CPU_ZERO( &set );
    CPU_SET( 0, &set );
    memset(&prio_param,0,sizeof(struct sched_param));
    if (sched_setaffinity( getpid(), sizeof( cpu_set_t ), &set )){
        perror( "sched_setaffinity" );
                exit(e_f);
    }
    if( (max_priority = sched_get_priority_max(SCHED_FIFO)) < 0 ){
                perror("sched_get_priority_max");
    }
    prio_param.sched_priority = max_priority;
    //if( sched_setscheduler(getpid(),SCHED_FIFO,&prio_param) < 0 ){
      //          perror("sched_setscheduler");
                //exit(e_f);
   //}
   int     nbytes,i=0;
   int     firstpipe[2],secondpipe[2],timepipe[2];
   char    parent_msg[] = "Hello from parent\n", child_msg[] = "Hello from child\n";
   char    readbuffer[100] ,tempbuffer[100];

   struct  timeval start,end;
   int fp = pipe(firstpipe), sp = pipe(secondpipe), tp = pipe(timepipe) ; 
   if (fp == -1) {
   	fprintf(er);
        return -1;
   }
   if (sp == -1) {
   	fprintf(er);
        return -1;
   }
   if (tp == -1) {
   	fprintf(er);
        return -1;
  }
  int f = fork();
  if(f==-1)
  perror("fork");
  else if(f==0){
  int n=-1;
        for(n=0;n<20;n++){
        nbytes = read(firstpipe[0], readbuffer, sizeof(readbuffer));
                write(secondpipe[1], child_msg, strlen(child_msg)+1);
        }
        gettimeofday(&end,0);
        n = sizeof(struct timeval);
if( write(timepipe[1],&end,sizeof(struct timeval)) != n ){
        fprintf(er);
                exit(e_f);
        }
  }else{
   	double context_switch_time;
        int n=-1;
        gettimeofday(&start,0);
        for(n=0;n<20;n++){
        write(firstpipe[1],parent_msg, strlen(parent_msg)+1);
                read(secondpipe[0], tempbuffer, sizeof(tempbuffer));
        }
        n = sizeof(struct timeval);
        if( read(timepipe[0],&end,sizeof(struct timeval)) != n ){
                fprintf(er);
                exit(e_f);
        }
        wait(NULL);
        context_switch_time = ((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))/1000.0;
        printf("context switch between two processes: %0.6lfms\n",context_switch_time/(40));
    }   
    return 0;
}

