#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

time_t mytime;
struct tm *timeinfo;
int keys[] = {0,1,2,3,4,5,6,7,8,9,10};
int pids[] = {0,0,0,0,0,0,0,0,0,0,0}; 
int foo(const int whoami) {
    printf("I'm waiting for key %d.  My pid is:%d\n", whoami, getpid());
    pids[whoami] = getpid();
    return 1;
}

int search_key(int key){
  int i;
  for(i=0;i<11;i++)
    if(keys[i]==key)
      return i;
  return -1;
}

void waiting_key(){
  int tmp, res;
  int i;
  FILE *f = fopen("pid.data", "rb");
  fread(pids, sizeof(int), sizeof(pids), f);
  fclose(f);
  printf("curr pid:%d\n", getpid());

  while(1){
    scanf("%d", &tmp);
    res = search_key(tmp);
    if(res!= -1){
      if((kill(pids[res], SIGKILL)) == 0){
				time(&mytime);
				timeinfo = localtime(&mytime);
	      printf("Process %d was killed at %s", (pids[res]), asctime(timeinfo));
			}
   	}
  }
}

int func(int n) {
  int i;
  if(fork()==0){
  for (i=0;i<n;i++){
    int pid = fork(); 
    if (pid==0){ 
      FILE *fr = fopen("pid.data", "rb");
      fread(pids, sizeof(int), sizeof(pids), fr);
      fclose(fr);
      pids[i] = getpid();
      FILE *f = fopen("pid.data", "wb");
      fwrite(pids, sizeof(int), sizeof(pids), f);
      fclose(f);
      foo(i);
      
      while(1){
      }
    }
    else{
      sleep(1);
      continue;
    }
  }
  exit(0);}
  return 0;

  
}


int main(){
  //if (fork() == 0)
  FILE *fr = fopen("pid.data", "wb");
  fwrite(pids, sizeof(int), sizeof(pids), fr);
  fclose(fr);
  func(10);
  sleep(11);
  waiting_key();
  return 0;
}
