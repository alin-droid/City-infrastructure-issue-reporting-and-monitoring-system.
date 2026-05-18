#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 200

void stop_monitor(){


    int fd = open(".monitor_pid", O_RDONLY);

    if (fd < 0) {
        return;
    }

    char buffer[20];
    int n = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (n <= 0) {
        return;
    }

    buffer[n] = '\0';

    pid_t pidMonitor = atoi(buffer);

     printf("pidMoniroe:%d\n",pidMonitor);

    if (pidMonitor > 0) {
        kill(pidMonitor, SIGINT);
    }

}


void start_monitor() {

    // creez procesul hub_mon
    pid_t hub_mon = fork();

    if (hub_mon < 0) {
        perror("error creating hub_mon");
        exit(-1);
    }

    if (hub_mon == 0) {
        // suntem in procesul copil

        int pipefd[2];

        // pipefd[0] - citire
        // pipefd[1] - scriere
        if (pipe(pipefd) < 0) {
            perror("error creating pipe");
            exit(-1);
        }

        // facem fork pentru monitor_reports
        pid_t monitor_pid = fork();

        if (monitor_pid < 0) {
            perror("error creating monitor_pid");
            exit(-1);
        }

        if (monitor_pid == 0) {
            //suntem in procesul copil

            // monitorul doar scrie in pipe, deci inchidem citirea
            close(pipefd[0]);

            //redirectionez stdout-ul monitorului in pipe
            dup2(pipefd[1], STDOUT_FILENO);

            // dupa dup2, nu mai am nevoie de pipefd[1]
            close(pipefd[1]);

            // inlocuiesc procesul actual cu monitor_reports
            execl("./monitor_reports", "monitor_reports", NULL);

            //ajunge aici doar daca execl esueaza
            perror("execl failed");
            exit(-1);

           
        }

        if (monitor_pid > 0) {
            // suntem in hub_mon
            int pidMonitor=getpid();
            // hub_mon doar citeste din pipe, deci inchidem scrierea
            close(pipefd[1]);

            char msgFromPipe[256];
            int msgSize;

            while ((msgSize = read(pipefd[0], msgFromPipe, sizeof(msgFromPipe) - 1)) > 0) {

                msgFromPipe[msgSize] = '\0';
                if(strcmp(msgFromPipe,"sigint  has been recived\n")==0){
                    
                }

                if (strstr(msgFromPipe, "error:") != NULL) {
                    printf("monitor had an unexpected problem: already running / cannot write in .monitor_pid\n");
                    break;
                }

                printf("from pipe: %s\n", msgFromPipe);
            }
            
            
            close(pipefd[0]);

            printf("pidMoniroe:%d\n",pidMonitor);

            exit(0);
        }
    }
    
}


void calculate_scores(char districts[MAX][MAX+1],int numOfdistricts){
     
    for(int i=0;i<numOfdistricts;i++){
        
        pid_t scorer=fork();

        if(scorer<0){
            printf("error at calculate_score from distict:%s\n",districts[i]);
            exit(-1);
        }

        if(scorer==0){
          //copil
        }

        if(scorer>0){
          //parinte

        }
    }

}
int main(int argc,char *argv[]){

    if(argc<2){
        printf("you have to introduce the rigth number of arguments!\n");
        exit(-1);
    }

    else if(strcmp(argv[1],"start_monitor")==0){

        start_monitor();

    }

    else if(strcmp(argv[1],"stop_monitor")==0){

        stop_monitor();

    }

    else if(strcmp(argv[1],"calculate_scores")==0){
        
        if(argc<3){
            printf("you have to introduce at least one district!\n");
        }

        int numOfDistricts=argc-1;
        char districts[MAX][MAX+1];
         
        int j=2;

        for(int i=0;i<numOfDistricts;i++){
            strcpy(districts[i],argv[j++]);
        }
        
        /*for(int  i=0;i<numOfDistricts;i++){
            printf("%s " , districts[i]);
        }*/

        //calculate_scores(districts,numOfDistricts);

    }

    else {
        printf("you need to introduce a rigth operation!\n");
    }

    return 0;
}