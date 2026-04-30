#include <stdio.h>    
#include <stdlib.h>   
#include <sys/types.h> 
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


void handler(int sig) {
  //sigusr1 se adauga!
    char msgAdd[]="added a new report\n";

    if(sig == SIGUSR1) {
        write(1, msgAdd, strlen(msgAdd));
    }
    //la asta se opreste
    
    char msgInterrupted[]="sigint  has been recived\n";
    if(sig == SIGINT) {
        write(1,msgInterrupted, strlen(msgInterrupted));
        //sterg fisierul
        unlink(".monitor_pid");
        exit(0);
    }
}

int main(){
  
  // O_CREAT - creeaza
  // O_WRONLY -write only
  //O_TRUNC sterge ce a fost scris

  int f= open(".monitor_pid", O_CREAT | O_WRONLY | O_TRUNC, 0644); 
  
  if(f==-1){
    printf("cannot acces .monitor_pid\n");
    exit(-1);
  }
  
  //optin pid-ul procesului
  pid_t pid = getpid();
  
  //scriu in .monitor pidul
  char pidBuffer[15];
  int lengthPid=sprintf(pidBuffer,"%d",pid);

  write(f,pidBuffer,lengthPid);
  close(f);

  //partea pt sigaction

   struct sigaction act={0};
   //pun functia mea de handler
   act.sa_flags = 0;
   act.sa_handler = &handler;

   sigaction(SIGUSR1, &act, NULL);
   sigaction(SIGINT, &act, NULL);
   
  while(1){
     
  }
  return 0;
}